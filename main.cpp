#include <bits/stdc++.h>
//#include <iostream>
//#include <stdio.h>
#define debug 0
#define stdio 1

using namespace std;

int R,C,N,P,Q,U,V,command;
long long K;
string input;
string inFileName, outFileName;
ifstream fin;
ofstream fout;

long long gcd2(long long X, long long Y) {
    long long tmp;
    while (X != Y && Y != 0) {
        tmp = X;
        X = Y;
        Y = tmp % Y;
    }
    return X;
}

struct grid{
    long long gcd;
    grid *left, *right;

    grid():gcd(0),left(0),right(0){}
    grid(long long a):gcd(a),left(0),right(0){}
};

ostream& operator<<(ostream& os, grid g)  {
    os << g.gcd;
    return os;
}
//1D segment tree
struct seg_1d{
    grid* root;
    seg_1d *left, *right;

    seg_1d():root(0), left(0), right(0){}
	void up(grid* current);
	void insert( int x, long long data, int l, int r, grid*& current );
	long long query( int a, int b, int l, int r, grid* current );
	void merge( int x, grid* a, grid* b, int l, int r, grid*& current );
};

void seg_1d::merge( int x, grid* a, grid* b, int l, int r, grid*& current ){
    if( r<x || x<l )return;

    if(!current)current=new grid();

	if(!a)current->gcd=b->gcd;
    else if(!b)current->gcd=a->gcd;
    else current->gcd = gcd2(a->gcd, b->gcd);

	if( l==r ) return;

	int mid = (l+r)/2;
	merge( x, ((!a)?0:a->left), ((!b)?0:b->left), l, mid, current->left);
	merge( x, ((!a)?0:a->right), ((!b)?0:b->right), mid+1, r, current->right);
}

void seg_1d::up(grid* current){
    if(!current->left)current->gcd=current->right->gcd;
    else if(!current->right)current->gcd=current->left->gcd;
    else current->gcd = gcd2(current->left->gcd, current->right->gcd);
}

void seg_1d::insert( int x, long long data, int l, int r, grid*& current ){
    if( r<x || x<l )return;

    if(!current)current=new grid();

	if( l==x && x==r ){
		current->gcd = data;
		return;
	}
	int mid = (l+r)/2;
	insert( x, data, l, mid, current->left );
	insert( x, data, mid+1, r, current->right );

	up(current);
}

long long seg_1d::query( int a, int b, int l, int r, grid* current ){
	if( current==0 || r<a || b<l ) return 0;
	if( a<=l && r<=b ) {return current->gcd;}

	int mid = (l+r)/2;
	long long sl = query( a, b, l, mid, current->left );
	long long sr = query( a, b, mid+1, r, current->right );
	return gcd2(sl, sr);
}
/********************************************************/
//2D segment tree, single point modification & area query
struct seg_2d{
	seg_1d* root;

	void insert( int y, int x, long long data, int ml, int mr, int nl, int nr, seg_1d*& current );
	long long query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, seg_1d* current );
};

long long seg_2d::query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, seg_1d* current ){
	if( current==0 || mr<y1 || y2<ml ) return 0;//Not included at all.
	if( y1<=ml && mr<=y2 )//Completely included, return.
		return current->query( x1, x2, nl, nr, current->root );

	int mid = (ml+mr)/2;//Partially included, recusive.
	long long sl = query( y1, y2, x1, x2, ml, mid, nl, nr, current->left );
	long long sr = query( y1, y2, x1, x2, mid+1, mr, nl, nr, current->right );
	return gcd2(sl, sr);
}

void seg_2d::insert( int y, int x, long long data, int ml, int mr, int nl, int nr, seg_1d*& current ){
    if( y<ml || mr<y ) return;//Not included at all.

    if(!current)current=new seg_1d();//Partially included, ++.

	if( y==ml && y==mr ) {//Completely included, return.
        current->insert( x, data, nl, nr, current->root );
        return;
    }
	int mid = (ml+mr)/2;//Partially included, recusive.
	insert( y, x, data, ml, mid, nl, nr, current->left );
	insert( y, x, data, mid+1, mr, nl, nr, current->right );

	current->merge( x, ( (!current->left)?0:current->left->root ), ( (!current->right)?0:current->right->root ), nl, nr, current->root);
}

seg_2d seg;

void solution1(){
    while(N--){
        if(!stdio)getline(fin, input);
        else getline(cin, input);
        stringstream ss(input);
        ss>>command;
        if(command==1){
            ss>>P>>Q>>K;
            seg.insert(Q, P, K, 0, C-1, 0, R-1, seg.root);
        }else if(command==2){
            ss>>P>>Q>>U>>V;
            if(!stdio)fout << seg.query(Q, V, P, U, 0, C-1, 0, R-1, seg.root) << "\n";
            else cout << seg.query(Q, V, P, U, 0, C-1, 0, R-1, seg.root) << "\n";
        }

    }
}

int main(int argc, char* argv[])
{
    if(!stdio){
        argv[1] = "01.random1";
        inFileName="E:/SystemFile/Desktop/CODING/C++/CompetitiveProgrammingTraining/ioi2013/game/game/"+string(argv[1])+".in";
        outFileName="E:/SystemFile/Desktop/CODING/C++/CompetitiveProgrammingTraining/ioi2013/game/game/out/"+string(argv[1])+".myout";
        fin.open(inFileName);
        fout.open(outFileName);
        cout << inFileName <<"\n"<<outFileName;
        fin >> R>>C>>N;
        getline(fin, input);
    }else{
        ios::sync_with_stdio(false);
        cin.tie(0);
        cin>>R>>C>>N;
        getline(cin, input);
    }
    solution1();
    return 0;
}


/*
Q:
http://www.ioi2013.org/competition/tasks/
https://tioj.infor.org/problems/1836
hint:
https://blog.masterliu.net/segment-tree-dynamic/
testcase:

*/
