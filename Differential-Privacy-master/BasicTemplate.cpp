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

const db EPS = 1e-8;

int sgn( db x) {return x < - EPS ? - 1 : x > EPS ;}
