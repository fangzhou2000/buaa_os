addi $a0, $0, -2020
addi $a1, $0, -2018
jal func
move $a0,$v0
li $v0,1
syscall
li $v0,10
syscall


func:

sw $fp, 0($sp)

addi $fp, $sp, 4

sw $ra, -8($fp)

add $t1, $zero, $a0

add $t2, $zero, $a1

bgez $t1, if_else_0

li $t8, 0

sub $t0, $t8, $t1

add $t0, $t0, 0

add $v0, $t2, $t0

move $sp, $fp

lw $fp, -4($sp)

lw $ra, -8($sp)

jr $ra

if_else_0:

if_end_0:

add $t0, $t1, -0

add $v0, $t2, $t0

move $sp, $fp

lw $fp, -4($sp)

lw $ra, -8($sp)

jr $ra