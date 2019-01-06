#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<ctime>
using namespace std;
const int inf = 999999999;
int A[10005],B[10005];
struct Splay
{
	Splay *ls,*rs,*fz;
	int size,data;
	Splay(Splay *father,int val):ls(0),rs(0),fz(father),size(1),data(val){};
}*root=0,*_none=new Splay(0,0);//define the Splay node, the usage of _none is the same to that in the AVL code 
void zig(Splay *x)//zig and zag is the same to those in the AVL code,no more comment.
{
    Splay *y=x->fz,*z=y->fz;
    if(y==z->ls)z->ls=x;else z->rs=x;
    x->fz=z;y->ls=x->rs;
    if(y->ls)y->ls->fz=y;
   	x->rs=y;y->fz=x;
    x->size=y->size;
    y->size=(y->ls?y->ls->size:0)+(y->rs?y->rs->size:0)+1;	
}
void zag(Splay *x)//zig and zag is the same to those in the AVL code,no more comment.
{
   	Splay *y=x->fz,*z=y->fz;
   	if(y==z->ls)z->ls=x;else z->rs=x;
   	x->fz=z;y->rs=x->ls;if(y->rs)y->rs->fz=y;
   	x->ls=y;y->fz=x;
   	x->size=y->size;
    y->size=(y->ls?y->ls->size:0)+(y->rs?y->rs->size:0)+1;
}
void splay(Splay *x)//splay operations.
{
   	Splay *y,*z;
    while(x->fz!=_none)//if x!=root ,it should go on being rotated up 
    {
    	y=x->fz;z=y->fz;
    	if(z!=_none)//if y is not the root,we would use doulbe rotaion
    	{
    	
    		if(z->ls==y)
    		{
    			if(y->ls==x){zig(y);zig(x);}
    			else {zag(x);zig(x);}
			}
			else
			{
				if(y->rs==x){zag(y);zag(x);}
				else {zig(x);zag(x);}
			}
    	}
    	else //if y is the root , use single rotaion 
    	{	
    		if(y->ls==x)zig(x);else zag(x);
    	}
    	//the detailed rotaion rules have been declared in the project report.
   	}
    root=x;//x has become the root.
}
Splay *find(int x)//alone the tree to find the vertex we want.
{
    Splay *p=root;
    while(p)
   	{
    	if(p->data==x)break;
    	if(x<p->data)p=p->ls;
    	else p=p->rs;
   	}
    return p;
}
void _insert(int x)
{
    if(root==0)//if the tree is empty,make the new element the root.
    {
    	root=new Splay(_none,x);
    	return;
    }
   	else
   	{
   		//start from root ,find the place first, the same to AVL code.
    	Splay *p=root;
    	while(p)
    	{   
    		if(x<p->data)
    		{
    			if(p->ls)p=p->ls;
    			else 
    			{
    				Splay *temp=new Splay(p,x);
    				p->ls=temp;
    				splay(temp);  return;
    			}
    		}
    		else
    		{
    			if(p->rs)p=p->rs;
    			else 
    			{
    				Splay *temp=new Splay(p,x);
    				p->rs=temp;
    				splay(temp);return;
    			}
    		}
    	}
    }
}
Splay *getmax(Splay *x)// get the max element in the sub-tree x.
{
	if(!x)return x;
	while(x->rs)x=x->rs;
	return x;
}
Splay *getmin(Splay *x)// get the min element in the sub-tree x.
{
	if(!x)return x;
	while(x->ls)x=x->ls;
	return x;
}
void _del(int x)//del in Splay is very simple.
{
    Splay *p=find(x);  //find the point to be deleted.
    if(p==0)cout<<"error!"<<endl;
    if(p)//if the point exist,del it.
    {
    	splay(p);//first make it the root .
    	Splay *L=p->ls,*R=p->rs;delete p; //delete it directly (much more direct than that in AVL or BST)
    	if(L==0){root=R;if(R)R->fz=_none;return;}//if it's left-subtree is empty, make the roof of the right-subtree the root.
    	else 
    	{
    		root=L;root->fz=_none;  
			//make left-subree's max element the root of the tree,thus that the root of it has no right child
    		//so we can directly make the right sub-tree of the origional tree the right son of the left-subtree. 
			Splay *temp=getmax(L);
    		splay(temp);
    		if(R)//update the information.
    		{
			  root->rs=R;R->fz=root;
    		  root->size+=R->size;
    	    }
			return;
        }
    }
}
/*int main()
{
	int T;int a,b,c,d,e;int n,m;
    ifstream t1_o1("type1_ordered1.in");ifstream t1_o2("type1_ordered2.in");ifstream t1_r("type1_randomed.in");
    ifstream t2_o1("type2_ordered1.in");ifstream t2_o2("type2_ordered2.in");ifstream t2_r("type2_randomed.in");
    ifstream t3_o1("type3_ordered1.in");ifstream t3_o2("type3_ordered2.in");ifstream t3_r("type3_randomed.in");
    ifstream t4_o1("type4_ordered1.in");ifstream t4_o2("type4_ordered2.in");ifstream t4_r("type4_randomed.in");
    e=0;
    double MIN,MAX,average;double start,end;
    
	MIN=inf;MAX=0;average=0;
	t1_o1>>T;
	for(a=1;a<=T;a++)
	{
		t1_o1>>n>>m;
		for(b=1;b<=n;b++)t1_o1>>A[b];
		for(b=1;b<=m;b++)t1_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
	    root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t1_o2>>T;
	for(a=1;a<=T;a++)
	{
		t1_o2>>n>>m;
		for(b=1;b<=n;b++)t1_o2>>A[b];
		for(b=1;b<=m;b++)t1_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t1_r>>T;
	for(a=1;a<=T;a++)
	{
		t1_r>>n>>m;
		for(b=1;b<=n;b++)t1_r>>A[b];
		for(b=1;b<=m;b++)t1_r>>B[b];
		start=clock();
		
		for(b=1;b<=n;b++)_insert(A[b]);
	    for(b=1;b<=m;b++)_del(B[b]);
		
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	

	
	MIN=inf;MAX=0;average=0;
	t2_o1>>T;
	
	for(a=1;a<=T;a++)
	{
		t2_o1>>n>>m;
		for(b=1;b<=n;b++)t2_o1>>A[b];
		for(b=1;b<=m;b++)t2_o1>>B[b];
		start=clock();

		
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		
		
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t2_o2>>T;
	for(a=1;a<=T;a++)
	{
		t2_o2>>n>>m;
		for(b=1;b<=n;b++)t2_o2>>A[b];
		for(b=1;b<=m;b++)t2_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;

	
	MIN=inf;MAX=0;average=0;
	t2_r>>T;
	for(a=1;a<=T;a++)
	{
		t2_r>>n>>m;
		for(b=1;b<=n;b++)t2_r>>A[b];
		for(b=1;b<=m;b++)t2_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_o1>>T;
	for(a=1;a<=T;a++)
	{
		t3_o1>>n>>m;
		for(b=1;b<=n;b++)t3_o1>>A[b];
		for(b=1;b<=m;b++)t3_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_o2>>T;
	for(a=1;a<=T;a++)
	{
		t3_o2>>n>>m;
		for(b=1;b<=n;b++)t3_o2>>A[b];
		for(b=1;b<=m;b++)t3_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_r>>T;
	for(a=1;a<=T;a++)
	{
		t3_r>>n>>m;
		for(b=1;b<=n;b++)t3_r>>A[b];
		for(b=1;b<=m;b++)t3_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t4_o1>>T;
	for(a=1;a<=T;a++)
	{
		t4_o1>>n>>m;
		for(b=1;b<=n;b++)t4_o1>>A[b];
		for(b=1;b<=m;b++)t4_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t4_o2>>T;
	for(a=1;a<=T;a++)
	{
		t4_o2>>n>>m;
		for(b=1;b<=n;b++)t4_o2>>A[b];
		for(b=1;b<=m;b++)t4_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t4_r>>T;
	for(a=1;a<=T;a++)
	{
		t4_r>>n>>m;
		for(b=1;b<=n;b++)t4_r>>A[b];
		for(b=1;b<=m;b++)t4_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)_insert(A[b]);
		for(b=1;b<=m;b++)_del(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
}*/ 
