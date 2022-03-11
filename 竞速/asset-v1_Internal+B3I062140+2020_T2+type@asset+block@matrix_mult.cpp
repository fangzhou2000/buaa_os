#include <iostream>
#include <cstdio>
#include <cstring>
#define test(x) printf("%d\n",&(x))
#include <cstdlib>
#include <time.h>

using namespace std;

int num = 0;
int index = 0;
int key = 8000;
int *myArr;
int random[10000];
const int maxn = 105;
struct Matrix {
    int n,m;
    int v[maxn][maxn];
    Matrix(int n,int m) : n(n), m(m) {}
    
    void init(){     
        memset(v, 0, sizeof v);
    }
    
    Matrix operator* (const Matrix B) const {
        Matrix C(n,B.m);
        C.init();
        for(int i = 0;i < n;i++)
            for(int j = 0;j < B.m;j++)
                for(int k = 0;k < m;k++) {
                    test(i);
                    test(i);
                    test(i);
                    test(j);
                    test(j);
                    test(j);
                    test(B.m);
                    test(k);
                    test(k);
                    test(k);
                    test(m);
                    test(i);
                    test(j);
                    test(i);
                    test(j);
                    test(k);
                    test(k);
                    test(C.v[i][j]);
                    test(B.v[k][j]);
                    test(v[i][k]);
                    C.v[i][j] += v[i][k]*B.v[k][j];
                }
        return C;
    }
    
    void print(){ 
        for(int i = 0;i < n;i++) {
            for(int j = 0;j < m;j++)
                cout << v[i][j] << " ";
            cout << endl;
        }
    }
};

int main() {
    srand((int)time(0));
    freopen("case.txt", "w", stdout);
    int n1 = 40,m1 =40 ,n2 = 40,m2 = 40;
    Matrix A(n1,m1);
    for(int i = 0;i < n1;i++)
        for(int j = 0;j < m1;j++) {
            test(i);
            test(j);
            test(i);
            test(j);
            test(i);
            test(j);
            test(n1);
            test(m1);
            test(A.v[i][j]);
            A.v[i][j]= (int)rand();
        }
    Matrix B(n2,m2);
    for(int i = 0;i < n2;i++)
        for(int j = 0;j < m2;j++) {
            B.v[i][j]= (int)rand();
            test(i);
            test(j);
            test(i);
            test(j);
            test(i);
            test(j);
            test(n2);
            test(m2);
            test(B.v[i][j]);
        }
    Matrix C = A*B;
    printf("-1\n");
    return 0;
}
