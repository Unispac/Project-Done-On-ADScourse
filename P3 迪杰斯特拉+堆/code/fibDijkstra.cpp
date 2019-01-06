#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<conio.h>
#include<cstdio>
#include<cmath>
#include<vector>
using namespace std;
const int maxn = 24000000;
const int maxm = 60000000;
char o[100]; FILE *file;
inline void _read(long long &x)
{
	char t = fgetc(file); bool k = 0;
	while ((t<'0' || t>'9') && t != '-')t = fgetc(file);
	if (t == '-')k = 1, t = fgetc(file);
	for (x = 0; t >= '0'&&t <= '9'; t = getc(file))x = x * 10 + t - '0';
	if (k)x = -x;
}
bool mark[maxn];
/*inline void _out(int x)
{
if (x == 0) { putchar('0'); return; }if (x<0) { putchar('-'); x = -x; }
int i;
for (i = 0; x != 0; i++, x /= 10)o[i] = x % 10 + '0';
for (i--; i >= 0; i--)putchar(o[i]);
}*/
struct Fib
{
	long long key;
	int degree, id;
	bool mark;
	Fib *p, *child, *left, *right;
};
struct list
{
	list *next;
	int aim;
	long long len;
};
Fib *M[50];
long long dis[maxn];
int st, ed;
Fib *P[maxn];
int n, m;

void swap(Fib *&x, Fib *&y)
{
	Fib *temp = x; x = y; y = temp; return;
}
Fib *makefib()
{
	Fib *x = new Fib;
	x->key = -1; x->degree = 0;//x->size=0;x->sum=0;
	x->mark = false; x->p = x->child = x->left = x->right = NULL;
	return x;
}
void shift(Fib *x, Fib *y)
{
	y->mark = false; 
	y->p = NULL;
	x->degree++;
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
{
	//x->size+=y->size;
	//	x->sum+=y->sum;
	shift(x, y);
}
Fib *mergetree(Fib *x, Fib *y)
{
	if (x->key>y->key)swap(x, y);
	x->degree += 1;
	//x->size+=y->size;
	//	x->sum+=y->sum;	
	y->mark = false; y->p = x;
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
Fib *mergeheap(Fib *x, Fib *y)
{

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
{
	int D = 30;
	//int D=ceil(log(x->size)/log(1.6));
	Fib *target = x->child, *temp;
	int i = x->degree, j;

	while (i)
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
void de_fix(Fib *H, Fib *x)
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
void de_key(Fib *H, Fib *x, long long k)
{

	if (k>x->key) { cout << "Can not change the key" << endl; }
	Fib *y;
	x->key = k; y = x->p;
	if (y != NULL && x->key<y->key)
	{
		cut(H, x, y);
		de_fix(H, y);
	}
	if (x->key<H->child->key)H->child = x;
	return;
}
void del(Fib *H, Fib *x)
{
	de_key(H, x, -999999999);
	pop(H);
	return;
}
list *HEAD[maxn];
list *TAIL[maxn];
void addpath(int x, int y, long long z)
{
	//cout << x << endl;
	if (HEAD[x] == NULL)
	{
		if (x < 0 || x >= maxn) { cout << x << endl; }
		HEAD[x] = new list();
		TAIL[x] = HEAD[x];
		if (HEAD[x] == NULL)cout << "error" << endl;
		HEAD[x]->next = NULL;
		HEAD[x]->len = z;
		HEAD[x]->aim = y;
	}
	else
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
void Dijkstra(int x)
{
	int a, b, c, d, e;
	long long s, t; long long len, now;
	for (a = 1; a <= n; a++)
	{
		P[a]=NULL;
		dis[a] = 999999999;
		//mark[a]=false;
	}

	Fib *root = makefib(); Fib *temp;
	temp = new Fib;
	temp->degree = 0; temp->child = temp->p = temp->left = temp->right = NULL;
	
	temp->key = 0; temp->id = x;
	P[x] = temp; dis[x] = 0;
	insert(root, temp);
	list *tem; mark[a] = true;

	//cout<<x<<" "<<root->degree<<" "<<root->child->id<<" "<<root->child->key<<endl; 

	while (root->degree)
	{
		temp = root->child;
		s = temp->id; now = temp->key;
		pop(root);
		if (mark[s] == true)continue;
		mark[s] = true;
		tem = HEAD[s];
		while (tem != NULL)
		{
			t = tem->aim; len = tem->len;
			if (dis[t]>dis[s] + len)
			{
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
}
int main()
{
	int a; long long b, c, d, e;
	file=fopen("USA-road-d.USA.gr", "r");
	//file=fopen("test.txt","r");
	fscanf(file, "%d%d", &n, &m);
	//cout << n << " " << m << endl;
	char t;
	for (a = 1; a <= m; a++)
	{
		//cout << a << endl;
		t = fgetc(file);
		while (t != 'a')t = fgetc(file);
		//fscanf_s(file, "%intd%intd%intd", &b, &c, &d);
		_read(b); _read(c); _read(d);
		/*if (a <=10)
		{
		cout << n << " " << m << " " << b << " " << c << " " << d << endl;
		}*/
		addpath(b, c, d);
		if (a %1000000==0)
		{
			cout<<a<<endl;
			//cout << b << " " << c << " " << d << endl;
		}
	}
	fclose(file);
	cout << "The MAP has been loaded!" << endl<<endl;
    
    cout<<"按1继续，按0退出"<<endl<<endl;
    t=getch();
    
	while(t=='1')
	{
		cout<<endl<<endl;
		cout<<"输入起点和终点"<<endl;
		scanf("%d%d", &st, &ed);
		//cout << st << "**********" << ed << endl;
		Dijkstra(st);
		cout<<endl<<"最短距离是:"<<endl;
		printf("%lld\n", dis[ed]);
		cout<<"按1继续，按0退出"<<endl<<endl;
        t=getch();
	}
	return 0;
}

