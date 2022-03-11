#include "lib.h"
#include <fs.h>

#define debug 0

static int file_close(struct Fd *fd);
static int file_read(struct Fd *fd, void *buf, u_int n, u_int offset);
static int file_write(struct Fd *fd, const void *buf, u_int n, u_int offset);
static int file_stat(struct Fd *fd, struct Stat *stat);

struct Dev devfile =
{
.dev_id=	'f',
.dev_name=	"file",
.dev_read=	file_read,
.dev_write=	file_write,
.dev_close=	file_close,
.dev_stat=	file_stat,
};


// Open a file (or directory),
// returning the file descriptor on success, < 0 on failure.
int
open(const char *path, int mode){
	struct Fd *fd;
	struct Filefd *ffd;
	u_int size, fileid;
	int r;
	u_int va;
	u_int i;

	// Step 1: Alloc a new Fd, return error code when fail to alloc.
	// Hint: Please use fd_alloc.


	// Step 2: Get the file descriptor of the file to open.
	// Hint: Read fsipc.c, and choose a function.


	// Step 3: Set the start address storing the file's content. Set size and fileid correctly.
	// Hint: Use fd2data to get the start address.


	// Step 4: Alloc memory, map the file content into memory.
	

	// Step 5: Return the number of file descriptor.
}

// Close a file descriptor
int
file_close(struct Fd *fd)
{
	// Your code here.
	int r;
	struct Filefd *ffd;
	u_int va,size,fileid;
	u_int i;

	ffd = (struct Filefd*)fd;
	fileid = ffd->f_fileid;
	size = ffd->f_file.f_size;
	va = fd2data(fd);       //the start address storing the file's content

	//tell the file server the dirty page.
	for(i = 0; i < size; i += BY2PG)
	{
		//our framework can't PTE_D
		//if(((* vpt)[(va+i)/BY2PG] & PTE_D)!=0)
			fsipc_dirty(fileid, i);
	}
	
	//request the file server to close the file
	if((r = fsipc_close(fileid))<0)
	{
		writef("cannot close the file\n");
		return r;
	}
	
	//unmap the content of file
	if(size == 0) return 0;
	
	for(i = 0; i < size; i +=BY2PG)
	{
		if((r = syscall_mem_unmap(0, va+i))<0)
		{
			writef("cannont unmap the file.\n");
			return r;
		}
	}

	//close the file descriptor	
	fd_close(fd);
	
	return 0;
	//user_panic("close() unimplemented!");
}

// Read 'n' bytes from 'fd' at the current seek position into 'buf'.
// Since files are memory-mapped, this amounts to a user_bcopy()
// surrounded by a little red tape to handle the file size and seek pointer.
static int
file_read(struct Fd *fd, void *buf, u_int n, u_int offset)
{
	u_int size;
	struct Filefd *f;
//	writef("file_read() come 1\n");
	f = (struct Filefd*)fd;

	// avoid reading past the end of file
	size = f->f_file.f_size;
	if (offset > size)
		return 0;
	if (offset+n > size)
		n = size - offset;

	// read the data by copying from the file mapping
//	writef("file_read(): bcopy(): src:%x  dst:%x  len:%x \n",(int)(char*)fd2data(fd)+offset, buf, n);
	
	user_bcopy((char*)fd2data(fd)+offset, buf, n);
//	writef("file_read() come 2\n");
	return n;
}

// Find the virtual address of the page
// that maps the file block starting at 'offset'.
int
read_map(int fdnum, u_int offset, void **blk)
{
	int r;
	u_int va;
	struct Fd *fd;

	if ((r = fd_lookup(fdnum, &fd)) < 0)
		return r;
	if (fd->fd_dev_id != devfile.dev_id)
		return -E_INVAL;
	va = fd2data(fd) + offset;
	if (offset >= MAXFILESIZE)
		return -E_NO_DISK;
//writef("offset=%x,      va=%x,  (* vpd)[PDX(va)]&PTE_P=%x,  (* vpt)[VPN(va)]&PTE_P=%x\n",offset,va,(* vpd)[PDX(va)]&PTE_V,(* vpt)[VPN(va)]&PTE_V);
	if (!((* vpd)[PDX(va)]&PTE_V) || !((* vpt)[VPN(va)]&PTE_V))
	{
		return -E_NO_DISK;
}
	*blk = (void*)va;
	return 0;
}

// Write 'n' bytes from 'buf' to 'fd' at the current seek position.
static int
file_write(struct Fd *fd, const void *buf, u_int n, u_int offset)
{
	int r;
	u_int tot;
	struct Filefd *f;

	f = (struct Filefd*)fd;

	// don't write past the maximum file size
	tot = offset + n;
	if (tot > MAXFILESIZE)
		return -E_NO_DISK;

	// increase the file's size if necessary
	if (tot > f->f_file.f_size) {
		if ((r = ftruncate(fd2num(fd), tot)) < 0)
			return r;
	}

	// write the data
	user_bcopy(buf, (char*)fd2data(fd)+offset, n);
	return n;
}

static int
file_stat(struct Fd *fd, struct Stat *st)
{
	struct Filefd *f;

	f = (struct Filefd*)fd;

	strcpy(st->st_name, f->f_file.f_name);
	st->st_size = f->f_file.f_size;
	st->st_isdir = f->f_file.f_type==FTYPE_DIR;
	return 0;
}

// Truncate or extend an open file to 'size' bytes
int
ftruncate(int fdnum, u_int size)
{
	int i, r;
	struct Fd *fd;
	struct Filefd *f;
	u_int oldsize, va, fileid;

	if (size > MAXFILESIZE)
		return -E_NO_DISK;

	if ((r = fd_lookup(fdnum, &fd)) < 0)
		return r;
	if (fd->fd_dev_id != devfile.dev_id)
		return -E_INVAL;

	f = (struct Filefd*)fd;
	fileid = f->f_fileid;
	oldsize = f->f_file.f_size;
	if ((r = fsipc_set_size(fileid, size)) < 0)
		return r;

	va = fd2data(fd);
	// Map any new pages needed if extending the file
	for (i = ROUND(oldsize, BY2PG); i < ROUND(size, BY2PG); i += BY2PG) {
		if ((r = fsipc_map(fileid, i, va+i)) < 0) {
			fsipc_set_size(fileid, oldsize);
			return r;
		}
	}

	// Unmap pages if truncating the file
	for (i = ROUND(size, BY2PG); i < ROUND(oldsize, BY2PG); i+=BY2PG)
		if ((r = syscall_mem_unmap(0, va+i)) < 0)
			user_panic("ftruncate: syscall_mem_unmap %08x: %e", va+i, r);
	return 0;
}

// Delete a file
int
remove(const char *path)
{
	// Your code here.
	// Call fsipc_remove.
}

// Synchronize disk with buffer cache
int
sync(void)
{
	return fsipc_sync();
}

