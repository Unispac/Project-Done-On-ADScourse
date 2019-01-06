#include<algorithm>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<cstdio>
#include<vector>
#include<cmath>
using namespace std;
FILE *file;int st,ed;
const long long maxm=30;//The upper bound of Bi_queue.
const long long maxn=24000000;//The upper bound of vertex
bool mark[maxn];long long dis[maxn];
//mark array shows weather x has been used to update other vertex's distance.
//dis array to memeorize the distance.
char o[100]; 
inline void _read(long long &x)//annual input.
{
	char t = fgetc(file); bool k = 0;
	while ((t<'0' || t>'9') && t != '-')t = fgetc(file);
	if (t == '-')k = 1, t = fgetc(file);
	for (x = 0; t >= '0'&&t <= '9'; t = getc(file))x = x * 10 + t - '0';
	if (k)x = -x;
}
struct MAP//using list to memorize the MAP.
{
	MAP *next;
	int aim;
	long long len;
};
struct Bitree; 
struct list//the child list of Bi_heap / Bi_tree
{
    Bitree *pt;
    list *next,*last;
    list()
    {
    	pt=NULL;next=last=NULL;
    }
};
struct Bitree//the structure of Bi_tree.
{
	int size,id;long long key;
	//size:the level of the tree.
	//id:the vertex's id in the map.
	//key:distance.
    list *head,*tail;//head and tail of child list.
    Bitree(long long cost)
    {
    	size=0;key=cost;head=tail=NULL;
	}
};
struct Bique//the structure of Bi_queue.
{
	int key;//Bique's key's shows the state of each
	long long MIN;
	list *head,*tail;
	Bique(long long cost)
	{
		key=1;MIN=cost;
		head=tail=new list;
		head->pt=new Bitree(cost);
	}
	Bique()	{key=0;MIN=0;head=tail=NULL;}
};
long long n,m;//vetex and arc's numbers.

void swap(Bitree *&x,Bitree *&y)//swap two tree.
{
	Bitree* temp=x;x=y;y=temp;return;
}
void swap(Bique *&x,Bique *&y)//swap two queues.
{
	Bique *temp=x;x=y;y=temp;return;
}
Bitree *mergetree(Bitree *x,Bitree *y)//merge two trees.
{
	if(x->key>y->key)swap(x,y);
	x->size++;//size of the tree ++;
	list *temp=new list;
	temp->pt=y;temp->next=NULL;temp->last=x->tail;
	//merge the two trees.
	if(x->head==x->tail&&x->head==NULL)
	{
		x->head=x->tail=temp;
	}
    else
    {
	   x->tail->next=temp;x->tail=temp;
    }
	return x;
}
Bique *mergeque(Bique *x,Bique *y)//merge two queues.
{
	if(x==NULL)return y;
	if(y==NULL)return x;
	
	if(x->key<y->key)swap(x,y);//merge the littile one into the bigger one.
	list *tx=x->head,*ty=y->head;
	
	x->key+=y->key;
    x->MIN=min(x->MIN,y->MIN);
	list *temp=NULL;
	//tx denotes the head of list x.
	//ty denotes the head of list y.
	while(ty!=NULL)//only need to merge their child lists.
	{
		
		while(tx!=NULL&&ty->pt->size>tx->pt->size)tx=tx->next;	
		//The merge can be divided into four different types of situation
		if(tx!=NULL&&tx->pt->size==ty->pt->size)
		{
			ty->pt=mergetree(ty->pt,tx->pt);
			
		    if(tx->last==NULL)x->head=tx->next;
		    else tx->last->next=tx->next;
		    if(tx->next==NULL)x->tail=tx->last;
			else tx->next->last=tx->last;
			temp=tx;tx=tx->next;
			delete temp;
			
			while(ty->next!=NULL&&ty->next->pt->size==ty->pt->size)
		    {
		    	ty->next->pt=mergetree(ty->next->pt,ty->pt);
		    	temp=ty;ty=ty->next;
		        delete temp;
			}
		}
		else if(tx!=NULL&&tx->pt->size!=ty->pt->size)
		{
			temp=ty->next;
			if(tx->last==NULL){x->head=ty;ty->last=NULL;ty->next=tx;tx->last=ty;}
			else {tx->last->next=ty;ty->last=tx->last;ty->next=tx;tx->last=ty;}
			ty=temp;
		}
		else if(tx==NULL)
		{
			temp=ty->next;
			if(x->head==x->tail&&x->head==NULL)
			{
				x->head=x->tail=ty;
				ty->next=NULL;
				ty->last=NULL;
			    tx=ty;
			}
			else if(x->tail!=NULL)
			{
			    x->tail->next=ty;
			    ty->last=x->tail;
			    ty->next=NULL;
			    x->tail=ty;
		        tx=ty;
			}
			ty=temp;
		}
	}
    delete y;
    return x;
}
Bique *pop(Bique *x,int &ID)//delete the top element.
{
	//first delete the tree including the min element from the queue.
	//then insert the tree's each son into a new queue.
	//last merge the three queues.
	if(x->key==0)return x;
    long long i,j;
    Bique *rt=new Bique;
	Bitree *target;
    list *temp=x->head;
	while(temp!=NULL)//linner searching the child list.
	{
		
		if(temp->pt->key==x->MIN)//find the Min element in the queue.
		{
			//delete it from the list.
			target=temp->pt;
			if(temp->last!=NULL)temp->last->next=temp->next;
			if(temp->next!=NULL)temp->next->last=temp->last;
			if(temp==x->head)
			{
				if(temp->next!=NULL)
				{
					x->head=temp->next;
				}
				else 
				{
					x->head=x->tail=NULL;
				}
			}
			else if(temp==x->tail)
			{
				if(temp->last!=NULL)
				{
					x->tail=temp->last;
				}
				else
				{
					x->head=x->tail=NULL;
				}
			}
			ID=temp->pt->id;
			delete temp;
			break;
		}
		else temp=temp->next;
	}
	i=target->size;
	x->key-=(1<<i);
	rt->key=(1<<i)-1;
	x->MIN=999999999;
	temp=x->head;
	while(temp!=NULL)//find the new min element of heap x.
	{
		x->MIN=min(x->MIN,temp->pt->key);
		temp=temp->next;
	}
	
    rt->MIN=999999999;
	rt->head=target->head;
	rt->tail=target->tail;
	temp=rt->head;
	while(temp!=NULL)//insert the deleted tree's child into a new queue.
	{
		rt->MIN=min(rt->MIN,temp->pt->key);
		temp=temp->next;
	}
	delete target;
	return mergeque(x,rt);//merge the two queues.
}

MAP *HEAD[maxn];
MAP *TAIL[maxn];

void addpath(int x, int y, long long z)//the same to Fibonaci heap,using list to record the map.
{
	if (HEAD[x] == NULL)
	{
		if (x < 0 || x >= maxn) { cout << x << endl; }
		HEAD[x] = new MAP();
		TAIL[x] = HEAD[x];
		if (HEAD[x] == NULL)cout << "error" << endl;
		HEAD[x]->next = NULL;
		HEAD[x]->len = z;
		HEAD[x]->aim = y;
	}
	else
	{
		if (TAIL[x] == NULL)cout << "error" << endl;
		TAIL[x]->next = new MAP();
		TAIL[x] = TAIL[x]->next;
		TAIL[x]->aim = y;
		TAIL[x]->len = z;
		TAIL[x]->next = NULL;
	}
	return;
}
void add(Bique *&x,int id,int len)//add a vertex into the heap.
{
	Bique *temp=new Bique(len);
	temp->head->pt->id=id;//build a new queue for the verte
	x=mergeque(x,temp);//merge the two queues.
	return;
}
void Dijkstra(int x)
{
	/*
	    Different with the Dijkstra with Fibonaci heap.
	    We use a mark array!
	    For in Bi_que,we can't aribitrarily change an elment in the queue.
	    Once we update a vertex's dis,we have to insert a new vertex in the queue again!!!
	    whicn means a vertex in the map may have mutiple vetexs in the heap denoting it.
	    So we must use a mark array to avoid repeatedly use a vertex to updating other vertex.
	*/
	int a, b, c, d, e;
	long long t; long long len, now;int s;
	for (a = 1; a <= n; a++)
	{
		mark[a]=false;dis[a] = 99999999999LL;
	}//initialize the array.
	Bique *root = new Bique();//make a new heap. 
	add(root,x,0);//add the start point into the heap.
	dis[x] = 0;
	MAP *tem;
	while (root->key)//Doing the algorithm untill the heap is empty.
	{
		now = root->MIN;
		root=pop(root,s);
		if(mark[s]==true)continue;
		mark[s]=true;
		
		tem = HEAD[s];
		while (tem != NULL)
		{
			t = tem->aim; len = tem->len;
			if (mark[t]==false&&dis[t]>dis[s] + len)
			{
				dis[t] = dis[s] + len;
			    add(root,t,dis[t]);
			}
			tem = tem->next;
		}
	}
	delete root;
}
int main()
{
	int a; long long b, c, d, e;
	file=fopen("USA-road-d.USA.gr", "r");//open the MAP file. 
	fscanf(file, "%d%d", &n, &m);
	char t;
	for (a = 1; a <= m; a++)
	{
		t = fgetc(file);
		while (t != 'a')t = fgetc(file);//get rid of 'a' in the file.
		
		_read(b); _read(c); _read(d);
		addpath(b, c, d);//add an arc between b and c.
		
		if (a %1000000==0)printf("%.0lf%\n",(double(a)/double(m))*100.0); //show the progress rate of loading map
	   
	}
	fclose(file);//close the file.
	
	
	//After the MAP has been laoded,we offer a on-line query interface.
	 
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
	
	/*
	   below is a test for 1000 queries.
	   As Dijkstra is a single source shortest-path algorithm.
	   Each time we solve a query,we should restart Dijkstra.
	   We use sum to memorize the total time,and sum/1000 is used to 
	   show the everage efficiency.
	   And MAX,MIN are used to record the max time and the min time of a single query
	   to show the stability of the Algorithm	
	
	file=fopen("in.txt", "r");
	long long beg,end;
	long long MAX=-9999999999LL;
	long long MIN=9999999999LL;
	double sum=0;
	
	for(a=1;a<=1000;a++)
	{
		fscanf(file,"%d%d",&st,&ed);
		beg=clock();
		Dijkstra(st);
		cout<<"Case "<<a<<" :"<<endl;
		cout<<"The ans is : "<<dis[ed]<<endl;
		end=clock();
		cout<<"Time cost : "<<end-beg<<endl<<endl;
		sum+=end-beg;
		MAX=max(MAX,end-beg);
		MIN=min(MIN,end-beg);
	}
	cout<<"The average is : "<< sum/1000.0<<endl;
	cout<<"The max is : "<< MAX<<endl;
	cout<<"The min is : "<< MIN<<endl;
	*/ 
	
	return 0;
}

