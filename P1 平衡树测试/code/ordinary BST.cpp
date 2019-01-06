#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<ctime>
using namespace std;
const int inf = 999999999;
int A[10005],B[10005];
struct BST
{
	BST *ls,*rs,*fz;
	int data;
	BST(BST *f,int x):fz(f),ls(0),rs(0),data(x){};
}*root=0;//define the BST's mumber. 
void _insert(int x)//insert the data,almost the same to AVL,but no need to trace_back.
{
	if(root==0)
	{
		root=new BST(0,x);
		return;
	}
	else
	{
		BST *p=root;
		while(p)
		{
			if(x>p->data)
			{
				if(p->rs)p=p->rs;
				else
				{
					p->rs=new BST(p,x);
					break;
				}
			}
			else
			{
				if(p->ls)p=p->ls;
				else
				{
					p->ls=new BST(p,x);
					break;
				}
			}
		}
	}
}
BST *find(BST *k,int x)//along the sub-tree k to find x.
{
	while(k)
	{
		if(k->data==x)break;
		if(x>k->data)k=k->rs;
		else k=k->ls;
	}
	return k;
}
BST *getmax(BST *x)//getmax
{
	while(x->rs)x=x->rs;
	return x;
}
void _del(BST *k,int x)//have the same method to AVL,but no need to trace back,so it is much easier.
{
	BST *p=find(k,x);
	if(!p)cout<<"error"<<endl;
	if(p)
	{
		if(p->ls==0)
		{
			if(p->rs==0)
			{
				if(p==root){root=0;delete p;}
		        else
		        {
		        	if(p->fz->ls==p)p->fz->ls=0;
					else p->fz->rs=0; 
					delete p;
		        }
  			}
  			else
  			{
  				if(p==root){root=p->rs;p->rs->fz=0;delete p;}
  				else
  				{
  					if(p->fz->ls==p)p->fz->ls=p->rs;
  					else p->fz->rs=p->rs;
  					p->rs->fz=p->fz;
  					delete p;
  				}
  			}
		}
		else if(p->rs==0)
		{
			if(p->ls==0)
			{
				if(p==root){root=0;delete p;}
		        else
		        {
		        	if(p->fz->ls==p)p->fz->ls=0;
					else p->fz->rs=0; 
					delete p;
		        }
  			}
  			else
  			{
  				if(p==root){root=p->ls;p->ls->fz=0;delete p;}
  				else
  				{
  					if(p->fz->ls==p)p->fz->ls=p->ls;
  					else p->fz->rs=p->ls;
  					p->ls->fz=p->fz;
  					delete p;
  				}
  			}
		}
		else
		{
			BST *t=getmax(p->ls);
			if(t==p->ls)
			{
				if(p==root){root=t;root->fz=0;root->rs=p->rs;p->rs->fz=root;delete p;}
			    else
			    {
			    	if(p->fz->ls==p)p->fz->ls=t;
			    	else p->fz->rs=t;
			    	t->fz=p->fz;
			    	t->rs=p->rs;
			    	t->rs->fz=t;
			    	delete p;
			    }
			}
			else
			{
				BST *h=new BST(0,t->data); _del(p->ls,t->data);
				if(p==root){root=h;h->fz=0;h->ls=p->ls;h->rs=p->rs;h->ls->fz=h;h->rs->fz=h;delete p;}
				else
				{
					if(p->fz->ls==p)p->fz->ls=h;else p->fz->rs=h;
					h->fz=p->fz;h->ls=p->ls;h->rs=p->rs;
					h->ls->fz=h;h->rs->fz=h;
					delete p; 
				}
			}
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
	    for(b=1;b<=m;b++)_del(root,B[b]);
		
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
		for(b=1;b<=m;b++)_del(root,B[b]);
		
		
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
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
		for(b=1;b<=m;b++)_del(root,B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		root=0;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	char t=getchar(); while(t!='.')t=getchar();
}*/ 
