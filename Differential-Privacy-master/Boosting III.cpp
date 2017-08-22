#include <iostream>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <cstring>
using namespace std;

#define mp make_pair
#define pb push_back
#define lson(x) ((x)<<1)
#define rson(x) ((x)<<1|1)

typedef double db;
typedef long long LL ;

const db EPS = 1e-1;
const int MAXN = (int)1e6 + 111;

int sgn( db x) {return x < - EPS ? - 1 : x > EPS ;}

/*
 * return double number in (x,x+1) with uniform distribution
 */

db Uniform(db x=.0){
	return x+(db)(1.0+rand())/(RAND_MAX+2.0);
}

/*
 * return double number with Laplace distribution(miu, lambda)
 */

db Lap( db miu = .0, db lambda = 1.0) {
	db U = Uniform(- 0.5) ;
	return miu - lambda * sgn(U) * log( fabs(1.0 - 2* fabs(U)) ) ;
}

/*
 * Add Lap( Delta / epsilon ) to a array A[], and store them on B[] (db)
 */

void AddLap( int A[], db B[], int n, db delta=1.0, db epsilon = 1.0) {
	for(int i = 0; i < n; ++ i) B[i] = A[i] + Lap( delta / epsilon) ;
}

class DPBinary{
private:
	int A[ MAXN ], n ;
	db delta, epsilon, lambda;
	db z[ MAXN << 2], h[ MAXN << 2],count[MAXN << 2], countwithnoise[ MAXN << 2];
public:
	db d0, d1;
	void setDelta(db d=1.0){delta = d;}
	void setEpsilon(db e=1.0) {epsilon=e;}
	void calLambda(){lambda = delta / epsilon;}
	db getLambda(){return lambda;}
	
	/*
	 * copy data from a[] to A[]
	 * here index from 0 to n-1 is the discrete domain
	 */
	
	void CopyData( int a[], int m) {
		n=m;
		for(int i=0;i<n;++i) A[i]=a[i];
	}
	
	int buildTree( int x, int l, int r) {
		int m=(l+r)>>1;
		if(l+1==r){
			count[x]=A[l];
			z[x]=countwithnoise[x] = count[x] + Lap(0, lambda);
			return 0;
		}
		int d=1;
		d+=buildTree(lson(x), l, m);
		buildTree(rson(x), m, r);
		count[x]=count[lson(x)]+count[rson(x)];
		countwithnoise[x] = count[x] + Lap(0, lambda);
		db c=(pow(2.,d-1.)-1.0)  /  (pow(2.,(db)d)-1);
		
		z[x] = (pow(2.,(db)d)-pow(2.,d-1.))  /  (pow(2.,(db)d)-1) * countwithnoise[x]
		+      c*(z[lson(x)]+z[rson(x)]);
		return d;
	}
	void _dfs(int x,int l, int r){
		int m=(l+r)>>1;
		if(x==1) {
			h[x]=z[x];
		}else {
			h[x] = z[x] + (h[x>>1]-z[lson(x>>1)]-z[rson(x>>1)]) * 0.5 ;
		}
		if(l+1 == r) return ;
		_dfs(lson(x),l,m);
		_dfs(rson(x),m,r);
	}
	void show(int x, int l, int r){
		printf("[%d,%d] %.3f(%.3f)\n", l, r-1, h[x],count[x]);
		if(l+1==r)return;
		int m=(l+r)>>1;
		show(lson(x),l,m);
		show(rson(x),m,r);
	}
	void CalError( int x, int l, int r){
		if(x==1) d0=d1=0.0;
		d0+=pow(count[x]-countwithnoise[x],2.0);
		d1+=pow(count[x]-h[x],2.0);
		int m=(l+r)>>1;
		if(l+1==r)return;
		CalError(lson(x), l, m);
		CalError(rson(x), m, r);
		if(x == 1) {
			cout <<" brute_force add Lap noise : Error  = " << sqrt(d0) << endl;
			cout <<" With consistency          : Error  = " << sqrt(d1) << endl;
		}
	}
};


const int MAXK = 4;
class DP123DTree{
private:
	int A[ MAXN ], n,las;
	db delta, epsilon, lambda;
	int nxt[ MAXN << 2 ][MAXK];
	int sz[ MAXN << 2];
	db h[ MAXN << 2] , count[MAXN << 2], countwithnoise[ MAXN << 2];
public:
	db d0, d1, gap;
	void setDelta(db d=1.0){delta = d;}
	void setEpsilon(db e=1.0) {epsilon=e;}
	void calLambda(){lambda = delta / epsilon;}
	db getLambda(){return lambda;}
	
	/*
	 * copy data from a[] to A[]
	 * here index from 0 to n-1 is the discrete domain
	 */
	
	void CopyData( int a[], int m) {
		n=m;
		for(int i=0;i<n;++i) A[i]=a[i];
	}
	
	void clr(int x){
		for(int i=0;i<MAXK;++i) nxt[x][i]=-1;
		count[x]=0; countwithnoise[x]=0;
	}
	int buildTree( int x, int l, int r) {
		if(x==1) las = 1;
		sz[x]=0;
		if(l+1==r){
			for(int i = 0; i <MAXK;++i) nxt[x][i]=-1; // no any sons
			count[x]=A[l];
			countwithnoise[x] = count[x] + Lap(0, lambda);
			h[x] = countwithnoise[x];
			return 0;
		}
		int d=1,len=r-l,cl,cr;
		count[x] = 0;
		
		if(len >= MAXK) {
			len /= MAXK;
			cl = l, cr = cl + len;
			for(int i = 0; i < MAXK; ++ i) {
				nxt[x][i]=++las; ++ sz[x];
				if(!i) d+= buildTree(las, cl, cr); else buildTree(las, cl, cr);
				count[x] += count[nxt[x][i]];
				cl = cr; cr = cl + len; if(i==MAXK-2) cr = r;
			}
		}else{
			for(int i=0;i<MAXK;++i) if(i<len){
				nxt[x][i]=++las; ++ sz[x];
				if(!i) d+= buildTree(las, l+i, l+i+1); else buildTree(las, l+i, l+i+1);
				count[x] += count[nxt[x][i]];
			}else nxt[x][i]=-1;
		}
		countwithnoise[x] = count[x] + Lap(0, lambda);
		h[x] = countwithnoise[x];
		return d;
	}
	
	db _dfs(int x){
		db sum = 0.0;
		for(int i = 0; i < MAXK;++i) if( nxt[x][i] != -1){
			sum += h[ nxt[x][i] ];
		}
		for(int i = 0; i < MAXK;++i) if( nxt[x][i] != -1){
			h[ nxt[x][i]]+= (h[x]-sum)/(sz[x]+1.0);
		}
		if(sz[x] > 1)
			h[x]=(sz[x]*h[x]+sum)/(sz[x]+1.0);
		
		for(int i = 0; i < MAXK;++i) if( nxt[x][i] != -1){
			_dfs(nxt[x][i]);
		}
	}
	void show(int x, int l, int r){
		printf("[%d,%d] %.3f(%.3f)\n", l, r-1, h[x],count[x]);
		if(l+1==r)return;
		int len=r-l,cl,cr;
		
		if(len >= MAXK) {
			len /= MAXK;
			cl = l, cr = cl + len;
			for(int i = 0; i < MAXK; ++ i) {
				show(nxt[x][i], cl, cr);
				
				cl = cr; cr = cl + len; if(i==MAXK-2) cr = r;
			}
		}else{
			for(int i=0;i<MAXK;++i) if(nxt[x][i] != -1){
				show(nxt[x][i], l+i, l+i+1);
			}
		}
	}
	
	void CalError( int x, int l, int r){
		if(x==1) d0=d1=gap=0.0;
		d0+=pow(count[x]-countwithnoise[x],2.0);
		d1+=pow(count[x]-h[x],2.0);
		if(l+1==r)return;
		int len=r-l,cl,cr;
		db sum = 0.0;
		if(len >= MAXK) {
			len /= MAXK;
			cl = l, cr = cl + len;
			for(int i = 0; i < MAXK; ++ i) {
				CalError(nxt[x][i], cl, cr); sum += h[ nxt[x][i]];
				cl = cr; cr = cl + len; if(i==MAXK-2) cr = r;
			}
		}else{
			for(int i=0;i<MAXK;++i) if(nxt[x][i] != -1){
				CalError(nxt[x][i], l+i, l+i+1); sum += h[ nxt[x][i]];
			}
		}
		gap=max(gap, fabs(h[x]-sum));
		if(x == 1) {
			/*cout <<" brute_force add Lap noise : Error  = " << sqrt(d0) << endl;
			cout <<" With consistency          : Error  = " << sqrt(d1) << endl;*/
			cout <<"now MAX_GAP = " << gap << endl;
		}
	}
	bool check( int x){
		if(sz[x] == 0) return true ;
		db sum = 0.0;
		for(int i = 0; i < MAXK; ++ i) if( nxt[x][i] != -1){
			sum += h[ nxt[x][i] ] ;
			if(!check( nxt[x][i])) return false ;
		}
		return sgn(sum - h[x]) == 0;
	}
}DK;

int A[ MAXN ], n ;

bool gen() {
	n = 100;
	for(int i = 0; i < n; ++ i) A[i] = Uniform() * 100;
	return true ;
}

int main(){
	srand( time( NULL )) ;
	int T = 1;
	while(T --) {
		gen();
		//for(int i=0;i<n;++i) cout << A[i] <<' '; cout << endl;
		DK.CopyData(A, n);
		DK.setEpsilon(0.01); DK.setDelta( log(n) / log(MAXK+.0));
		DK.calLambda();
		DK.buildTree(1, 0, n);
		int ct = 0, c;
		for(c = 0; ct == 0; ++ c) {
			DK._dfs(1);
			//DK.show(1, 0, n);
			DK.CalError(1, 0, n);
			ct = DK.check(1);
			//cout << (ct?"Correct!" : "No!") << endl;
		}
		cout <<" for n = " << n <<" c = " << c << endl;
	}
	return 0;
}
