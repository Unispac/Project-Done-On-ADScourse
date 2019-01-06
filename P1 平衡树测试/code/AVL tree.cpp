#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<ctime>
#define note (x->ls->height-x->rs->height)  //use note to calculate the balanced factor of x;
#define notee (y->ls->height-y->rs->height) //use notee to calculate the balanced factor of y; 
using namespace std;
const int inf = 999999999;//define a huge number 
int A[10005],B[10005];// A is used to store the number to be inserted and B is used to store those to be deleted.
struct AVL
{
	int num,height;
	AVL *ls,*rs,*fz;
	AVL(int data,int h,AVL *f,AVL *l,AVL *r):num(data),height(h),fz(f),ls(l),rs(r){};//initialization
}*_none=new AVL(0,-1,0,0,0),*root=_none;//define the AVL's mumber.
//_none is used to denote the unexisted vertex.Use it can avoid wrong operations on a NULL pointer.
void zig(AVL *x)//a single right rotation
{
	AVL *y,*z;
	y=x->fz;z=y->fz;  
	if(y==z->ls)z->ls=x;else z->rs=x;//make x the new son of z;
	x->fz=z;y->ls=x->rs;y->ls->fz=y;//make z the new father of x and make x's right son the new left son of y;
	x->rs=y;y->fz=x;//make y the new right son of x and make x the new father of y;
	y->height=max(y->ls->height,y->rs->height)+1;
	x->height=max(x->ls->height,x->rs->height)+1;//update x and y 's height.
    if(x->fz==_none)root=x; //if x is the new root of the tree,change the root
}
void zag(AVL *x)//a single left rotaion ,as it is symmetric to zig,no more comment.
{
	AVL *y,*z;
	y=x->fz;z=y->fz;
	if(y==z->ls)z->ls=x;else z->rs=x;
	x->fz=z;y->rs=x->ls;y->rs->fz=y;
	x->ls=y;y->fz=x;
	y->height=max(y->ls->height,y->rs->height)+1;
	x->height=max(x->ls->height,x->rs->height)+1;
    if(x->fz==_none)root=x;
}
void trace_back(AVL *p,AVL *y)// after insertion ,we need to updating the height alone the tracing back path.
{
	AVL *z=_none;
	while(p!=_none)//trace back from the node p;
	{
	    p->height=max(p->ls->height,p->rs->height)+1;//update p's height first.
		if(abs(p->ls->height-p->rs->height)>1)//if p is unbalanced.
		{
			if(y==p->ls)
			{
				if(z==_none||z==y->ls){zig(y);}
			    else{zag(z);zig(z);}
			}
			else
			{
				if(z==_none||z==y->rs){zag(y);}
				else{zig(z);zag(z);}
			}//according to the path,we can decide the rotaion type.  
		}
		z=y;y=p;p=p->fz;//p ,y and z is to store the recent 3 point for choosing the rotaion type.
	}
}
void _insert(int x)
{
	if(root==_none)// if the tree is empty, just make the inserted node the root;
	{
		root=new AVL(x,0,_none,_none,_none);
		return;
	}
	else
	{
		//search the inserting position from root.
		AVL *p=root;AVL *y;
		while(p!=_none)
		{
			if(x<p->num)//if x is lesser than the point we are now in ,go left.
			{
				if(p->ls!=_none)p=p->ls;//we haven't get the position,just go
				else
				{
					//we have arrived ,insert it.
					p->ls=y=new AVL(x,0,p,_none,_none);
					break;
				}
			}
			else //x is greater than the point we arrived, go right
			{
				//the same to the situation above
				if(p->rs!=_none)p=p->rs;
			    else
			    {
			    	p->rs=y=new AVL(x,0,p,_none,_none);
			    	break;
			    }
			}
		}
		trace_back(p,y);//trace_back to update the information
	}
}
AVL *getmax(AVL *x)//go along the right direction to get the max element of a sub-tree.
{
	while(x->rs!=_none)x=x->rs;
	return x;
}
AVL *getmin(AVL *x)// go along the left direction to get the min element of a sub-tree.
{
	while(x->ls!=_none)x=x->ls;
	return x;
}
void _del(int val)//delete the element val.
{
	AVL *p=root;
	//start from the root to search our point.
	while(p!=_none)
	{
		if(p->num==val)//have found
		{
			/*
			    when we find the point,how to delete it?
			    An executable way is to find a point in it's subtree to replace it.
			    Then the point to be deleted is actually gone.
			    the aim is exchanged into deleting another point we used to replace the former one.
			*/
			if(p->ls==_none&&p->rs==_none)break;//if a point is already a leaf point,delete it directly. 
			else
			{
				/*
				    pick a higher sub-tree to find the point we use to replace the origional point.
				*/
				if(p->ls->height>=p->rs->height)
				{
					AVL *t=getmax(p->ls);
					p->num=t->num;
					val=t->num;p=p->ls;
				}
				else
				{
					AVL *t=getmin(p->rs);
					p->num=t->num;
					val=t->num;p=p->rs;
				}
			}
		}
		else//haven't found yet;
		{  
		    //search along the tree.
		    if(val>p->num)p=p->rs;
		    else p=p->ls; 
	    }
	}
	//after the deletion,we are now in the bottom of the tree.
	//although the point we planned to delete may be in the middle of the tree,but by copy and transt our aim.
	//the tree actually lost a leaf point.
	//then we just need to trace back from the leaf to update.
	AVL *x,*y,*z;
	if(p->fz->ls==p)p->fz->ls=_none;
	else p->fz->rs=_none;
	x=p->fz;  delete p;
	while(true)
	{
	    if(x==_none)break;//trace finished.
		x->height=max(x->ls->height,x->rs->height)+1;//update the height.
		if(note==-1||note==1)break;
		//easy to show that note==+1/-1,the height of the sub-tree remain unchanged ,no need to go back anymore. 
		if(note==0)x=x->fz;//go on tracing back 
	    else if(note==2)//if the sub-tree is unbalanced, we need to locate the unbalanced path,to determin the rotation type.
	    {
	    	y=x->ls;
	    	if(notee==0){zig(y);break;}
	    	else if(notee==1){zig(y);x=y->fz;}
	    	else if(notee==-1){z=y->rs;zag(z);zig(z);x=z->fz;} 
	    }
	    else if(note==-2)
	    {
	    	y=x->rs;
	    	if(notee==0){zag(y);break;}
	    	else if(notee==-1){zag(y);x=y->fz;}
	    	else if(notee==1){z=y->ls;zig(z);zag(z);x=z->fz;}
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
	    root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
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
		root=_none;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
}*/
