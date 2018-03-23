#include <bits/stdc++.h>
//#include <iostream>
//#include <stdio.h>
#define debug 0
#define stdio 0

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

	void up(grid* current);
	void insert( int x, long long data, int l, int r, grid*& current );
	long long query( int a, int b, int l, int r, grid* current );
	void merge( int x, grid*& a, grid*& b, int l, int r, grid*& current );
};

void seg_1d::merge( int x, grid*& a, grid*& b, int l, int r, grid*& current ){
    if( r<x || x<l )return;
    if(debug)cout << "merge at x="<<l<<"~"<<r<<"x="<<x<<endl;
    if(current==0){//NULL
        current=new grid();
    }
    if(a==0)a=new grid();
    if(!b)b=new grid();

	current->gcd = gcd2(a->gcd, b->gcd);
	if( l==r ) return;
	int mid = (l+r)/2;

	merge( x, a->left, b->left, l, mid, current->left);
	merge( x, a->right, b->right, mid+1, r, current->right);
}

void seg_1d::up(grid* current){
    if(!current->left)current->gcd=current->right->gcd;
    else if(!current->right)current->gcd=current->left->gcd;
    else current->gcd = gcd2(current->left->gcd, current->right->gcd);
}

void seg_1d::insert( int x, long long data, int l, int r, grid*& current ){
    if( r<x || x<l )return;
    if(debug)cout << "insert at x="<<l<<"~"<<r<<"x="<<x<<endl;
    if(current==0){
        current=new grid();
    }
    if(debug)cout<<"@";

	if( l==x && x==r ){
        if(debug)cout<<"9";
		current->gcd = data;
        if(debug)cout<<"2";
		return;
	}
	if(debug)cout<<"!";
	int mid = (l+r)/2;
	insert( x, data, l, mid, current->left );
	insert( x, data, mid+1, r, current->right );

	up(current);
}

long long seg_1d::query( int a, int b, int l, int r, grid* current ){
	if( current==0 || r<a || b<l ) return 0;
	if(debug)cout << "query at x="<<l<<"~"<<r<<endl;
	if( a<=l && r<=b ) {return current->gcd;}
	int mid = (l+r)/2;
	long long sl = query( a, b, l, mid, current->left );
	long long sr = query( a, b, mid+1, r, current->right );
	return gcd2(sl, sr);
}
/********************************************************/
//2D segment tree, single point modification & area query
struct seg_2d{
	static const int MAXM = 2000;
	vector<seg_1d> st;
    vector<int> leftIndex, rightIndex;
    int dsize=0, root=0;
	void insert( int y, int x, long long data, int ml, int mr, int nl, int nr, int& d );
	long long query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, int d );
};

long long seg_2d::query( int y1, int y2, int x1, int x2, int ml, int mr, int nl, int nr, int d=1 ){
	if( d==0 || mr<y1 || y2<ml ) return 0;//Not included at all.
	if( y1<=ml && mr<=y2 )//Completely included, return.
		return st[d].query( x1, x2, nl, nr, st[d].root );
	int mid = (ml+mr)/2;//Partially included, recusive.
	long long sl = query( y1, y2, x1, x2, ml, mid, nl, nr, leftIndex[d] );
	long long sr = query( y1, y2, x1, x2, mid+1, mr, nl, nr, rightIndex[d] );
	return gcd2(sl, sr);
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

	st[d].merge( x, st[leftIndex[d]].root, st[rightIndex[d]].root, nl, nr, st[d].root);
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
            if(!stdio)fout << seg.query(Q, V, P, U, 0, C-1, 0, R-1, seg.root) << endl;
            else cout << seg.query(Q, V, P, U, 0, C-1, 0, R-1, seg.root) << endl;
        }

    }
}

int main(int argc, char* argv[])
{
    //argv[1] = "01.random1";
    if(!stdio){
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
