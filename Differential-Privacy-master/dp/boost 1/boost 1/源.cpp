#include <iostream>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <time.h>
#include <cstring>
using namespace std;

#define mp make_pair
#define pb push_back
#define lson(x) ((x)<<1)
#define rson(x) ((x)<<1|1)

typedef double db;
typedef long long LL;

const db EPS = 1e-8;
const int MAXN = (int)1e5 + 111;

int sgn(db x) { return x < -EPS ? -1 : x > EPS; }

/*
* return double number in (x,x+1) with uniform distribution
*/

db Uniform(db x = .0) {
	return x + (db)(1.0 + rand()) / (RAND_MAX + 2.0);
}

/*
* return double number with Laplace distribution(miu, lambda)
*/

db Lap(db miu = .0, db lambda = 1.0) {
	db U = Uniform(-0.5);
	//cout << "U: " << U << endl;
	return miu - lambda * sgn(U) * log(fabs(1.0 - 2 * fabs(U)));
}

/*
* Add Lap( Delta / epsilon ) to a array A[], and store them on B[] (db)
*/

void AddLap(int A[], db B[], int n, db delta = 1.0, db epsilon = 1.0) {
	for (int i = 0; i < n; ++i) B[i] = A[i] + Lap(delta / epsilon);
}

class DPCons {
private:
	int A[MAXN], n;
	db delta, epsilon, lambda;
	db z[MAXN << 2], h[MAXN << 2], count[MAXN << 2], countwithnoise[MAXN << 2];
public:
	db d0, d1;
	void setDelta(db d = 1.0) { delta = d; }
	void setEpsilon(db e = 1.0) { epsilon = e; }
	void calLambda() { lambda = delta / epsilon; }
	db getLambda() { return lambda; }

	/*
	* copy data from a[] to A[]
	* here index from 0 to n-1 is the discrete domain
	*/

	void CopyData(int a[], int m) {
		n = m;
		for (int i = 0; i<n; ++i) A[i] = a[i];
	}

	int buildTree(int x, int l, int r) {
		int m = (l + r) >> 1;
		if (l + 1 == r) {
			count[x] = A[l];
			z[x] = countwithnoise[x] = count[x] + Lap(0, lambda);
			return 0;
		}
		int d = 1;		//d denotes level of node in the tree
		d += buildTree(lson(x), l, m);
		buildTree(rson(x), m, r);
		count[x] = count[lson(x)] + count[rson(x)];
		countwithnoise[x] = count[x] + Lap(0, lambda);
		db c = (pow(2., d - 1.) - 1.0) / (pow(2., (db)d) - 1);

		z[x] = (pow(2., (db)d) - pow(2., d - 1.)) / (pow(2., (db)d) - 1) * countwithnoise[x] + c * (z[lson(x)] + z[rson(x)]);	//in the front of the equation, it denotes the 
		return d;
	}
	void _dfs(int x, int l, int r) {
		int m = (l + r) >> 1;
		if (x == 1) {
			h[x] = z[x];
		}
		else {
			h[x] = z[x] + (h[x >> 1] - z[lson(x >> 1)] - z[rson(x >> 1)]) * 0.5;
		}
		if (l + 1 == r)
			return;
		_dfs(lson(x), l, m);
		_dfs(rson(x), m, r);
	}
	void show(int x, int l, int r) {
		printf("[%d,%d] %.3f(%.3f)\n", l, r - 1, h[x], count[x]);
		if (l + 1 == r)return;
		int m = (l + r) >> 1;
		show(lson(x), l, m);
		show(rson(x), m, r);
	}
	void CalError(int x, int l, int r) {	//output error in consistency and brute-force add lap noise
		if (x == 1) d0 = d1 = 0.0;
		d0 += pow(count[x] - countwithnoise[x], 2.0);
		d1 += pow(count[x] - h[x], 2.0);
		int m = (l + r) >> 1;
		if (l + 1 == r)return;
		CalError(lson(x), l, m);
		CalError(rson(x), m, r);
		if (x == 1) {
			cout << " brute_force add Lap noise : Error  = " << sqrt(d0) << endl;
			cout << " With consistency          : Error  = " << sqrt(d1) << endl;
		}
	}
}DP;

int A[MAXN], n;

bool gen() {
	n = rand() % 100000 + 1;
	//for (int i = 0; i < n; ++i) A[i] = Uniform() * 100;
	for (int i = 0; i < n; ++i) A[i] = i + 5000;
	return true;
}

int main() {
	srand(time(NULL));
	int ty = 5;
	while (ty--) {
		gen();
		cout << "n = " << n << endl;
		DP.CopyData(A, 4);
		DP.setDelta(1.0);
		DP.setEpsilon(1);
		DP.calLambda();
		DP.buildTree(1, 0, 4);
		DP._dfs(1, 0, 4);
		DP.show(1, 0, 4);
		DP.CalError(1, 0, 4);
	}
	return 0;
}