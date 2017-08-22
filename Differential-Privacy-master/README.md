Differential-Privacy
====================

Differential Privacy


####Boosting The Accuracy of DP Through Consistency
######好多是自己理解的不一定对

其中有一部分生成树的代码
```cpp
int buildTree( int x, int l, int r) {
		int m = (l + r) >> 1;
		if(l + 1 == r){
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
		z[x] = (pow(2., (db)d) - pow(2., d - 1.)) / (pow(2., (db)d) - 1) * countwithnoise[x] + c * (z[lson(x)] + z[rson(x)]);
		return d;
	}
```
其中变量c的作用是计算权重，在二叉树的层中，层数越高，权重越大，同时，c的大小不能超过$\frac{1}{2}$，另一部分的权重在
```cpp
z[x] = (pow(2., (db)d) - pow(2., d - 1.)) / (pow(2., (db)d) - 1) * countwithnoise[x] + c * (z[lson(x)] + z[rson(x)]);
```
这一行中计算，其中

```cpp
(pow(2., (db)d) - pow(2., d - 1.)) / (pow(2., (db)d) - 1)
```
这一部分为
```cpp
1 - c
```
的值。

就是说把一棵二叉树中，的父节点定义为两个子节点的和（带上噪声），然后在数组z[]中，重新计算父节点的值。

下图用来说明变量之间的关系，其中方框中好像没法打括号，就用"\"代替了。。。
~~~mermaid
graph TD
subgraph 二叉树
A[countwithnoisea = lson + rson + noise] --> |lson|B[lson]
A --> |rson|C[rson]
end

subgraph z数组的生成
Z[z] --> S{sum }
S -->|* c| partone[lson + rson]
S -->|* \1 - c\| parttwo[countwithnoise]
end
~~~

















