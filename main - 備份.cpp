#include <bits/stdc++.h>
//#include <iostream>
//#include <stdio.h>
#define debug 0

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

    grid(){
        gcd=0;
    }
    grid(long long a){
        gcd=a;
    }
    grid operator+(grid N){
        return grid(gcd2(gcd, N.gcd));
    }
};

ostream& operator<<(ostream& os, grid g)  {
    os << g.gcd;
    return os;
}
//1D segment tree
struct seg_1d{
	static const long long MAXN = 100;
	vector<grid> st;
    vector<int> leftIndex, rightIndex;
    int dsize=0, root=0;
	void up(int d);
	void insert( int x, long long data, int l, int r, int& d );
	grid query( int a, int b, int l, int r, int d );
	void merge( int x, seg_1d &a, seg_1d &b, int l, int r, int& d, int dl, int dr );
};

void seg_1d::merge( int x, seg_1d &a, seg_1d &b, int l, int r, int& d, int dl, int dr ){
    if( r<x || x<l )return;
    if(dsize+100>=st.size()){
        st.resize(st.size()+100);
        leftIndex.resize(leftIndex.size()+100, 0);
        rightIndex.resize(rightIndex.size()+100, 0);
    }
    if(a.dsize+5>=a.st.size()){
        a.st.resize(a.st.size()+10);
        a.leftIndex.resize(a.leftIndex.size()+10, 0);
        a.rightIndex.resize(a.rightIndex.size()+10, 0);
    }
    if(b.dsize+5>=b.st.size()){
        b.st.resize(b.st.size()+10);
        b.leftIndex.resize(b.leftIndex.size()+10, 0);
        b.rightIndex.resize(b.rightIndex.size()+10, 0);
    }
	if(d==0)d = ++dsize;

	st[d] = a.st[dl]+b.st[dr];
	if( l==r ) return;
	int mid = (l+r)/2;

    int tempIndex=leftIndex[d];
	merge( x, a, b, l, mid, tempIndex, a.leftIndex[dl], b.leftIndex[dr] );
	leftIndex[d] = tempIndex;
	tempIndex=rightIndex[d];
	merge( x, a, b, mid+1, r, tempIndex , a.rightIndex[dl], b.rightIndex[dr] );
	rightIndex[d] = tempIndex;
}

void seg_1d::up(int d){
	st[d] = st[leftIndex[d]]+st[rightIndex[d]];
}

void seg_1d::insert( int x, long long data, int l, int r, int& d ){
    if( r<x || x<l )return;

    if(dsize+100>=st.size()){
        st.resize(st.size()+100);
        leftIndex.resize(leftIndex.size()+100, 0);
        rightIndex.resize(rightIndex.size()+100, 0);
    }

    if(d==0)d = ++dsize;
	if( l==x && x==r ){
		st[d].gcd = data;
		return;
	}
	int mid = (l+r)/2;
	int tempIndex=leftIndex[d];
	insert( x, data, l, mid, tempIndex );
	leftIndex[d] = tempIndex;
	tempIndex=rightIndex[d];
	insert( x, data, mid+1, r, tempIndex );
	rightIndex[d] = tempIndex;

	up(d);
}

grid seg_1d::query( int a, int b, int l, int r, int d=1 ){
	if( d==0 || r<a || b<l ) return grid();
	if( a<=l && r<=b ) {return st[d];}
	int mid = (l+r)/2;
	grid sl = query( a, b, l, mid, leftIndex[d] );
	grid sr = query( a, b, mid+1, r, rightIndex[d] );
	return sl+sr;
}
/********************************************************/
//2D segment tree, single point modification & area query
struct seg_2d{
	static const int MAXM = 2000;
	vector<seg_1d> st;
    vector<int> leftIndex, rightIndex;
    int dsize=0, root=0;
	void insert( int y, int x, long long data, int ml, int mr, int nl, int nr, int& d );
	grid query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, int d );
};

grid seg_2d::query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, int d=1 ){
	if( d==0 || mr<y1 || y2<ml ) return grid();//Not included at all.
	if( y1<=ml && mr<=y2 )//Completely included, return.
		return st[d].query( x1, x2, nl, nr );
	int mid = (ml+mr)/2;//Partially included, recusive.
	grid sl = query( y1, y2, x1, x2, ml, mid, nl, nr, leftIndex[d] );
	grid sr = query( y1, y2, x1, x2, mid+1, mr, nl, nr, rightIndex[d] );
	return sl+sr;
}

void seg_2d::insert( int y, int x, long long data, int ml, int mr, int nl, int nr, int& d ){
    if( y<ml || mr<y ) return;//Not included at all.

    if(dsize+1000>=st.size()){
        st.resize(st.size()+1000);
        leftIndex.resize(leftIndex.size()+1000, 0);
        rightIndex.resize(rightIndex.size()+1000, 0);
    }

    if(d==0)d = ++dsize;
	//Partially included, ++.
	if( y==ml && y==mr ) {
        st[d].insert( x, data, nl, nr, st[d].root );
        return;
    }//Completely included, return.
	int mid = (ml+mr)/2;//Partially included, recusive.
	int tempIndex=leftIndex[d];
	insert( y, x, data, ml, mid, nl, nr, tempIndex );
	leftIndex[d] = tempIndex;
	tempIndex=rightIndex[d];
	insert( y, x, data, mid+1, mr, nl, nr, tempIndex );
	rightIndex[d] = tempIndex;

	st[d].merge( x, st[leftIndex[d]], st[rightIndex[d]], nl, nr, st[d].root, st[leftIndex[d]].root, st[rightIndex[d]].root );
}

seg_2d seg;

void solution1(){
    while(N--){
        getline(fin, input);
        stringstream ss(input);
        ss>>command;
        if(command==1){
            ss>>P>>Q>>K;
            seg.insert(Q, P, K, 0, C-1, 0, R-1, seg.root);
        }else if(command==2){
            ss>>P>>Q>>U>>V;
            fout << seg.query(Q, V, P, U, 0, C-1, 0, R-1, seg.root) << endl;
        }

    }
}

int main(int argc, char* argv[])
{
    inFileName="E:/SystemFile/Desktop/CODING/C++/CompetitiveProgrammingTraining/ioi2013/game/game/"+string(argv[1])+".in";
    outFileName="E:/SystemFile/Desktop/CODING/C++/CompetitiveProgrammingTraining/ioi2013/game/game/out/"+string(argv[1])+".myout";
    fin.open(inFileName, std::ifstream::in);
    fout.open(outFileName);
    cout << inFileName <<"\n"<<outFileName;
    fin >> R>>C>>N;
    getline(fin, input);
    solution1();
    //while(1){}
    return 0;
}


/*
Q:
http://www.ioi2013.org/competition/tasks/
hint:
https://blog.masterliu.net/segment-tree-dynamic/
testcase:

comp command:
E:
cd E:\SystemFile\Desktop\CODING\C++\CompetitiveProgrammingTraining\ioi2013\game\game
PAUSE
for %i in (*) do (
start "game" "E:\SystemFile\Desktop\CODING\C++\CompetitiveProgrammingTraining\ioi2013\game\bin\Debug\game.exe" "%~ni"
)
PAUSE
for %i in (*) do (
start "winmerge" "E:\SystemFile\Desktop\¶w∏À¿…≠Ã\WinMerge-2.14.0-exe\winmergeu.exe" /f "Exclude Source Control" "out\%~ni.myout" "out\%~ni.out"
)
*/
