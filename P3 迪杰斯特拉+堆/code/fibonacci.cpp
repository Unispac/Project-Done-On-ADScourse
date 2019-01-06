#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<conio.h>
#include<cstdio>
#include<ctime>
#include<cmath>
#include<vector>
using namespace std;
const int maxn = 24000000;//The upper bound of vertex
const int maxm = 60000000;//The upper bound of arc
char o[100];//for mannual input
FILE *file;//point to the  map file
inline void _read(long long &x)//mannual input 
{
	char t = fgetc(file); bool k = 0;
	while ((t<'0' || t>'9') && t != '-')t = fgetc(file);
	if (t == '-')k = 1, t = fgetc(file);
	for (x = 0; t >= '0'&&t <= '9'; t = getc(file))x = x * 10 + t - '0';
	if (k)x = -x;
}
struct Fib//The structure of Fibonacii heap
{
	long long key;
	int degree, id;
	bool mark;
	Fib *p, *child, *left, *right;
};
struct list//Using list to memorize the map;
{
	list *next;
	int aim;
	long long len;
};
Fib *M[50];//the degree array 
long long dis[maxn];//dis array of Dijkstra 
int st, ed;//start point and end point
Fib *P[maxn];//Every vertex in the MAP have a pointer pointing to a vertex in the Fibonaci heap. 
int n, m;//the number of vertex and of arc.
void swap(Fib *&x, Fib *&y)//swap two fib.
{
	Fib *temp = x; x = y; y = temp; return;
}
Fib *makefib()//bulid a new heap.
{
	Fib *x = new Fib;
	x->key = -1; x->degree = 0;//x->size=0;x->sum=0;
	x->mark = false; x->p = x->child = x->left = x->right = NULL;
	return x;
}
void shift(Fib *x, Fib *y)//add tree y in heap x.
{
	y->mark = false; 
	y->p = NULL;
	x->degree++;
	//first add in to the child list.
	//then update the min mumber of x.
	if (x->child == NULL)
	{
		x->child = y;
		y->left = y->right = y;
	}
	else
	{
		x->child->right->left = y;
		y->right = x->child->right;
		x->child->right = y;
		y->left = x->child;
		if (y->key<x->child->key)x->child = y;
	}
}
void insert(Fib *x, Fib *y)
//as in this problem,we don't need to care about size/sum of a heap
//insert equals to shift.
{
	//x->size+=y->size;
	//	x->sum+=y->sum;
	shift(x, y);
}
Fib *mergetree(Fib *x, Fib *y)//merge two tree
{
	if (x->key>y->key)swap(x, y);//we should make the father is the samllest.
	x->degree += 1;
	//x->size+=y->size;
	//	x->sum+=y->sum;	
	y->mark = false; y->p = x;
	//insert y's child in x's child list and updata the min pointer.
	if (x->child == NULL)
	{
		x->child = y;
		y->left = y->right = y;
	}
	else
	{
		x->child->right->left = y;
		y->right = x->child->right;
		x->child->right = y;
		y->left = x->child;
		if (x->child->key>y->key)x->child = y;
	}
	return x;
}
Fib *mergeheap(Fib *x, Fib *y)//merge two heaps.
{
//insert y's child in x's child list and updata the min pointer.
	if (x == NULL)return y;
	if (y == NULL)return x;
	if (x->degree == 0) { delete x; return y; }
	if (y->degree == 0) { delete y; return x; }
	Fib *a, *b, *c, *d;
	
	a = x->child; b = a->right;
	c = y->child; d = c->right;
	a->right = d; d->left = a;
	c->right = b; b->left = c;

	if (x->child->key>y->child->key)x->child = y->child;
	x->degree += y->degree;//x->size+=y->size;x->sum+=y->sum;
	int i = x->degree; Fib *k = x->child;
	while (i)
	{
		k = k->right;
		i--;
	}

	delete y;
	return x;
}

void pop_fix(Fib *x)
//after pop an element in the heap and make it's child the new tree of heap x.
//we should do a liner search of x's tree merge them and find the min tree.
{
	int D = 30;
	//we can also use:
	//int D=ceil(log(x->size)/log(1.6));
	//but according to our tests,just use a number c is faster!
	
	Fib *target = x->child, *temp;
	int i = x->degree, j;

	while (i)//do a liner search among the childs of the heap.And merge them untill no tree's degree is the same.
	{
		temp = target; target = target->right;
		j = temp->degree;

		while (M[j] != NULL)
		{
			temp = mergetree(temp, M[j]);
			M[j] = NULL; j++;
		}
		;
		M[j] = temp;
		i--;
	}
	x->child = NULL;
	x->degree = 0;
	//using the degree array,reset the degree and child list of heap x.
	for (i = 0; i <= D; i++)
	{
		if (M[i] != NULL)
		{
			x->degree++;
			if (x->child == NULL)
			{
				x->child = M[i];
				M[i]->left = M[i]->right = M[i];
				M[i] = NULL;
			}
			else
			{
				x->child->right->left = M[i];
				M[i]->right = x->child->right;
				x->child->right = M[i];
				M[i]->left = x->child;
				if (M[i]->key<x->child->key)x->child = M[i];
				M[i] = NULL;
			}
		}
	}
	return;
}
void pop(Fib *x)
//delete the top elment of the heap,and insert it's child into the child list of heap x.
{
	if (x->child == NULL) { return; }
	Fib *target = x->child; Fib *temp = target->child; int i = target->degree;
	while (i)
	{
		Fib *next = temp->right;
		shift(x, temp);
		temp = next; i--;
	}
	target->left->right = target->right;
	target->right->left = target->left;
	x->degree--;//x->sum-=target->key;x->size--;
	if (target->right == target)x->child = NULL;
	else x->child = target->right;
	//P[target->id]=NULL;
	delete target;
	pop_fix(x);
	return;
}
void cut(Fib *H, Fib *x, Fib *y)
//cut the vertex x off vertex y in heap H. 
{
	y->degree--;//y->size-=x->size;y->sum-=x->sum;
	if (x == y->child)
	{
		if (x->right == x)y->child = NULL;
		else
		{
			y->child = x->right;
			x->right->left = x->left;
			x->left->right = x->right;
		}
		shift(H, x);
	}
	else
	{
		x->right->left = x->left;
		x->left->right = x->right;
		shift(H, x);
	}
	return;
}
void de_fix(Fib *H, Fib *x)//after we cut x's child,we should updata x's mark.
{
	if (x->mark == false)x->mark = true;
	else
	{
		Fib *y = x->p;
		if (y != NULL)
		{
			cut(H, x, y);
			de_fix(H, y);
		}
		else
		{
			x->mark = false;
		}
	}
}
void de_key(Fib *H, Fib *x, long long k)//decrease x in H to y.
{

	if (k>x->key) { cout << "Can not change the key" << endl; }
	//we can't increase x's key!
	Fib *y;
	x->key = k; y = x->p;
	//change the key,and then fix it's influence.
	if (y != NULL && x->key<y->key)
	{
		cut(H, x, y);
		de_fix(H, y);
	}
	if (x->key<H->child->key)H->child = x;
	//update the min pointer of H.
	return;
}
void del(Fib *H, Fib *x)//del vertex x in H.Although it is not used in this project.
{
	de_key(H, x, -999999999);//first make the vertex the top of the heap.
	pop(H);//delete the top.
	return;
}
list *HEAD[maxn];
list *TAIL[maxn];
//we use list to memorize the heap.
//HEAD[x]:point to the first arc starting from x.
//TAIL[x]:point to the last arc starting from x.
void addpath(int x, int y, long long z)
{
	//cout << x << endl;
	if (HEAD[x] == NULL)//if the list is empty,build the list.
	{
		if (x < 0 || x >= maxn) { cout << x << endl; }
		HEAD[x] = new list();
		TAIL[x] = HEAD[x];
		if (HEAD[x] == NULL)cout << "error" << endl;
		HEAD[x]->next = NULL;
		HEAD[x]->len = z;
		HEAD[x]->aim = y;
	}
	else//else,add the new arc in the list.
	{
		if (TAIL[x] == NULL)cout << "error" << endl;
		TAIL[x]->next = new list();
		TAIL[x] = TAIL[x]->next;
		TAIL[x]->aim = y;
		TAIL[x]->len = z;
		TAIL[x]->next = NULL;
	}
	return;
}
void Dijkstra(int x)//Dijkstra algorithm
{
	//Dijkstra algorithm with Fibonaci heap is very efficient.
	//For updating the key of a vertex in the heap just take O(1)!!!!
	
	int a, b, c, d, e;
	long long s, t; long long len, now;
	for (a = 1; a <= n; a++)
	{
		P[a]=NULL;dis[a] = 99999999999LL;
	}//initialize the array.
	Fib *root = makefib();//bulid the empty heap.
	Fib *temp;
	temp = new Fib;
	temp->degree = 0; temp->child = temp->p = temp->left = temp->right = NULL;
	temp->key = 0; temp->id = x;
	P[x] = temp; dis[x] = 0;
	//add the start point into the heap.
	insert(root, temp);
	list *tem;
	while (root->degree)
	{
		temp = root->child;
		s = temp->id; now = temp->key;
		pop(root);
		tem = HEAD[s];
		while (tem != NULL)//access the map list of vetex s.
		{
			t = tem->aim; len = tem->len;
			if (dis[t]>dis[s] + len)//updating the dis array.
			{
				//if s is not in the heap,we bulid a vetex in the heap for it.
				//else we just decrease the key of it.
				dis[t] = dis[s] + len;
				if (P[t] == NULL)
				{
					P[t] = new Fib;
					P[t]->left = P[t]->right = P[t]->p = P[t]->child = NULL;
					P[t]->degree = 0;
					P[t]->id = t;
					P[t]->key = dis[t];
					insert(root, P[t]);
				}
				else de_key(root, P[t], dis[t]);
			}
			tem = tem->next;
		}
	}
	delete root;
    //delete the root.
    //as the while loop won't stop untill the heap is empty.
    //when the while loop ends,other vertexs in the heap has been deleted.
    //so there can't be memory leak.
}


