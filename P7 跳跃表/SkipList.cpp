#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<cstdio>
#include<cmath>
using namespace std;
const int inf=999999999;//define a sufficiently large number inf. 

const int maxn=1000000;
int A[maxn],B[maxn];
/*
	The array A and B is used to memorize the test data.
	maxn denotes the max size of the data.
*/

class SkipList   //The class of the data structure
{
private:
	struct node //define the list node in the class.
	{
		int key;int content; 
		//key is used to sort the node.
		//content is the information we put in this node.
		node *left,*right,*up,*down;
		//four pointers point to the adjacent nodes.
		node()//the constructing funtion used to initialize the node.
		{
			left=right=up=down=NULL;
			key=content=0;
		}
	}*head;//head is the begining of the list.
	int MaxLevel;//MaxLevel denotes the max level of the list.
public:
	SkipList(int m);//Constructing the skip list of height m.
	~SkipList();
	//As there are dynamic memory,we need a destructor to free the
	//memeory in heap. 
	void Insert(int,int);
	//insert a node 
	void Delete(int);
	//delete a node 
	int Find(int);
	//find the node.
};
SkipList::SkipList(int m)//Construct a Skiplist of height m.
{
	MaxLevel=m;
	head=new node;//create the head element.
	for(int a=2;a<=m;a++)//create the other m-1 levels.
	{
		head->up=new node;
		head->up->down=head;
		head=head->up;
	}
	//The head element has no meaning.
	//It doesn't memorize anything.
	//Just be as a guider.
}
SkipList::~SkipList()//clear the heap memory.
{
	int x=0;
	node *p,*temp;
	while(head->down)head=head->down;
	while(head)//clear it level by level.
	{
		temp=head->up;
		while(head)
		{
			p=head->right;
			delete head;
			head=p;
		}
		head=temp;
	}
}
void SkipList::Insert(int key,int content)//Insert an element with key and content.
{
	node *pre[MaxLevel+1];int level=MaxLevel;
	pre[MaxLevel]=head;
	/*
		The array pre is used to memorize the neigbor 
		of the element in each level,which can be convenient when  we
		insert the element in these levels at last.
		
		The variable level records the level we are in currently.
	*/
	
	node *p=head;//make p points to the start of the list.
	while(true)//Find the position we need to insert the element.
	{
		if(p->right&&p->right->key==key)
		{
			cout<<"There has already been the same key!"<<endl;
			return;
		}//If we find there has already been the same key.Don't insert it.
		else if(p->right&&p->right->key<key){p=p->right;pre[level]=p;}
		//If the right node's key is lesser than the element go on moving along the right path.
		else if(p->down){p=p->down;level--;pre[level]=p;}
		//Else,We just go down.
		else break;
		//If we are already at the button then stop.
	}
	
	int k=rand()%2;level=1;
	while(level<MaxLevel&&k==1){level++;k=rand()%2;}
	/*
		Toss the coin.
		Randomly determine which level the element should be in. 
	*/
	
	node *temp=new node;
	temp->key=key;temp->content=content;
	temp->right=p->right;
	temp->left=p;
	p->right=temp;
	if(temp->right)temp->right->left=temp;
	//Insert the element into the button list.
	for(int a=2;a<=level;a++)//Insert it into other levels.
	{
		p=pre[a];//Find the position we should insert in light of the array pre.
		temp->up=new node;
		temp->up->down=temp;
		temp=temp->up;
		temp->key=key;temp->content=content;
		
		temp->right=p->right;
		temp->left=p;
		p->right=temp;
		if(temp->right)temp->right->left=temp;
	}
}
void SkipList::Delete(int key)//Delete a node.
{
	/*
	 	As delete is almost the same to the insert operation.
	 	Here we won't make specific comment any more.
	 	The method is easy.
	 	First find the element along the list.
	 	Then delete the element from all levels of it.
	*/
	node *p=head;
	while(true)
	{
		if(p->right&&p->right->key==key)
		{
			p=p->right;
			break;
		}
		else if(p->right&&p->right->key<key)p=p->right;
		else if(p->down)p=p->down;
		else break;
	}
	if(p&&p->key==key)
	{
		p->left->right=p->right;
		if(p->right)p->right->left=p->left;
		while(p->up)
		{
			p=p->up;
			delete p->down;
			p->left->right=p->right;
			if(p->right)p->right->left=p->left;
		}
		delete p;
	}
	else 
	{
		cout<<"Can't find it!"<<endl;
		return;
	}
}
int SkipList::Find(int key)
{
	/*
		Find operation is very easy.
		We have used it in the two operations above.
		So we won't make specific comment any more.
	*/
	node *p=head;
	while(true)
	{
		if(p->right&&p->right->key==key)
		{
			return p->right->content;
		}
		else if(p->right&&p->right->key<key)p=p->right;
		else if(p->down)p=p->down;
		else break;
	}
	cout<<"Can't find it!"<<endl;
	return -1;
}
/*
	Below is the part for data test.
	You can generate data with the {make data.cpp}.
*/

int main()
{
	srand((unsigned)time(NULL));
	int T;int a,b,c,d,e;int n,m;
    ifstream t1_o1("type1_ordered1.in");ifstream t1_o2("type1_ordered2.in");ifstream t1_r("type1_randomed.in");
    ifstream t2_o1("type2_ordered1.in");ifstream t2_o2("type2_ordered2.in");ifstream t2_r("type2_randomed.in");
    ifstream t3_o1("type3_ordered1.in");ifstream t3_o2("type3_ordered2.in");ifstream t3_r("type3_randomed.in");
    ifstream t4_o1("type4_ordered1.in");ifstream t4_o2("type4_ordered2.in");ifstream t4_r("type4_randomed.in");
    e=0;
    double MIN,MAX,average;double start,end;
    
    SkipList *p;
    
    
	MIN=inf;MAX=0;average=0;
	t1_o1>>T;
	for(a=1;a<=T;a++)
	{
		t1_o1>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t1_o1>>A[b];
		for(b=1;b<=m;b++)t1_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
	    delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t1_o2>>T;
	for(a=1;a<=T;a++)
	{
		t1_o2>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t1_o2>>A[b];
		for(b=1;b<=m;b++)t1_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t1_r>>T;
	for(a=1;a<=T;a++)
	{
		t1_r>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t1_r>>A[b];
		for(b=1;b<=m;b++)t1_r>>B[b];
		start=clock();
		
		for(b=1;b<=n;b++)p->Insert(A[b],0);
	    for(b=1;b<=m;b++)p->Delete(B[b]);
		
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;

	
	MIN=inf;MAX=0;average=0;
	t2_o1>>T;
	
	for(a=1;a<=T;a++)
	{
		t2_o1>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t2_o1>>A[b];
		for(b=1;b<=m;b++)t2_o1>>B[b];
		start=clock();

		
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t2_o2>>T;
	for(a=1;a<=T;a++)
	{
		t2_o2>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t2_o2>>A[b];
		for(b=1;b<=m;b++)t2_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;

	
	MIN=inf;MAX=0;average=0;
	t2_r>>T;
	for(a=1;a<=T;a++)
	{
		t2_r>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t2_r>>A[b];
		for(b=1;b<=m;b++)t2_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_o1>>T;
	for(a=1;a<=T;a++)
	{
		t3_o1>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t3_o1>>A[b];
		for(b=1;b<=m;b++)t3_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_o2>>T;
	for(a=1;a<=T;a++)
	{
		t3_o2>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t3_o2>>A[b];
		for(b=1;b<=m;b++)t3_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t3_r>>T;
	for(a=1;a<=T;a++)
	{
		t3_r>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t3_r>>A[b];
		for(b=1;b<=m;b++)t3_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	MIN=inf;MAX=0;average=0;
	t4_o1>>T;
	for(a=1;a<=T;a++)
	{
		t4_o1>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t4_o1>>A[b];
		for(b=1;b<=m;b++)t4_o1>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t4_o2>>T;
	for(a=1;a<=T;a++)
	{
		t4_o2>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t4_o2>>A[b];
		for(b=1;b<=m;b++)t4_o2>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
	
	
	MIN=inf;MAX=0;average=0;
	t4_r>>T;
	for(a=1;a<=T;a++)
	{
		t4_r>>n>>m;
		int level=int(log(n)/log(2));
		p=new SkipList(level);
		for(b=1;b<=n;b++)t4_r>>A[b];
		for(b=1;b<=m;b++)t4_r>>B[b];
		start=clock();
		for(b=1;b<=n;b++)p->Insert(A[b],0);
		for(b=1;b<=m;b++)p->Delete(B[b]);
		end=clock()-start;average+=end;
		MIN=min(end,MIN);MAX=max(end,MAX);
		delete p;
	}
	average/=(T*1.0);e++;
	cout<<"case"<<e<<": "<<"MIN="<<MIN<<" "<<"MAX="<<MAX<<" "<<"average="<<average<<endl;
}
