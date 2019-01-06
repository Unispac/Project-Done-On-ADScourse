#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<cmath>
using namespace std;

struct node
{
	int x,y,h;
	node *next,*last;
}*begin,*end,*p;
/*
	In our algorithm,we use a number of lines with different height to represent the box's state.
	After we put the objects on these line,the top viw of the boxes which consisits of some lines changes.
	These new lines are used to represent the new state!
	
	So we use the structure "node" to represent these lines.
	And use a list to manage them. 
*/
struct texture
{
	int width,height;
	texture *next,*last;
	int id;
}*head,*tail,*t;
/*
	The structure "texture" records the object.
	Use a list to manage them.
*/

int W,n;
/*
	W denotes the width of the box.
	n denotes the number of objects.
*/
int main()
{
	int a,b,c,d,e;
	cin>>n>>W;//get n and W.
	for(a=1;a<=n;a++)
	{
		t=new texture;
		t->id=a;
		if(a==1)
		{
			head=t;
			cin>>head->width>>head->height;
			head->last=NULL;
			head->next=NULL;
			tail=head;
		}
		else
		{
			cin>>t->width>>t->height;
			t->last=tail;
			t->next=NULL;
			tail->next=t;
			tail=t;
		}
	}
	
	begin=new node;
	begin->x=0;begin->y=W;begin->h=0;
	end=begin;
	begin->last=end->next=NULL;

	
	for(a=1;a<=n;a++)//We should put n objects.
	{
		/*
			According to our algorithm,each time we would choose the object and the position which
			get the best value to put our objects.
			So we try each place and each objects to find the best value.
		*/
		node *chosennode;
		texture *chosentexture;
		node *chosentemp;
		
		double MAX=-999999;
		int type;
		
		/*
			Our algorithm use the greedy policy,so each time we put in an object,it must tightly be with
			a boadrder which is an object's boarderline or the box's boarderline.
			And an object may be left tightly with the boarder or right tightly with the boadrder,thus when we
			enumerate the place and the object we must cover both of the situations.
			Meanwhile,an object has two ways to be placed.We must consider the different placing methods!
		*/
		
		for(p=begin;p!=NULL;p=p->next)//Left closed!
		//enumerate the place.
		{
			
			if(p!=begin&&p->h>=p->last->h)continue;//Can't get in touch with the left boarder!Continue
			
			
			for(t=head;t!=NULL;t=t->next)//enumerate the objects
			{
				node *temp;
				node *limit;
				double degree;
				
				//1.The first way to place the object.
				for(limit=p->next;limit!=NULL;limit=limit->next)//find the right boarder.(it would be used in calculate the function value)
				{
					if(limit->h>p->h)break;
				}
				
				b=p->x+t->width;
				for(temp=p->next;temp!=NULL;temp=temp->next)
				{
					if(temp->x>=b)break;
					if(temp->h>p->h)break;
				}//Judege weather the object can be placed here,and how many lines are coverd!
				
				if(b<=W&&(temp==NULL||temp->x>=b))//if we can place the object here,then calculate it's value.
				{
					if(limit!=NULL)degree=1.0-(limit->x-b)*1.0/sqrt(t->width*t->height*1.0);
				 	else {degree=1.0-(W-b)*1.0/sqrt(t->width*t->height*1.0);}
					if(degree>MAX)//if the value is better than the best value we have gotten,use the new situation to replace it.
					{
						MAX=degree;
						chosennode=p;
						chosentexture=t;
						chosentemp=temp;
						type=1;
					}
				}
				
				//The second way to place the object!
				b=p->x+t->height;
				for(temp=p->next;temp!=NULL;temp=temp->next)
				{
					if(temp->x>=b)break;
					if(temp->h>p->h)break;
				}
				
				if(b<=W&&(temp==NULL||temp->x>=b))
				{
					if(limit!=NULL)degree=1.0-(limit->x-b)*1.0/sqrt(t->width*t->height*1.0);
				 	else degree=1.0-(W-b)*1.0/sqrt(t->width*t->height*1.0);
					if(degree>MAX)
					{
						MAX=degree;
						chosennode=p;
						chosentexture=t;
						chosentemp=temp;
						type=2;
					}
				}
			}
		}
		//Below is the situation we place the object right tightly,as it is almost the same to that above
		//No more commitment for this.
		for(p=end;p!=NULL;p=p->last)
		{
			if(p!=end&&p->h>=p->next->h)continue;
			
			for(t=head;t!=NULL;t=t->next)
			{
				node *temp;
				node *limit;
				double degree;
				
				b=p->y-t->width;
				
				for(limit=p->last;limit!=NULL;limit=limit->last)
				{
					if(limit->h>p->h)break;
				}
		
				for(temp=p->last;temp!=NULL;temp=temp->last)
				{
					if(temp->y<=b)break;
					if(temp->h>p->h)break;
				}
				if(b>=0&&(temp==NULL||temp->y<=b))
				{
					if(limit!=NULL)degree=1.0-(b-limit->y)*1.0/sqrt(t->width*t->height*1.0);
				 	else degree=1.0-b*1.0/sqrt(t->width*t->height*1.0);
					if(degree>MAX)
					{
						MAX=degree;
						chosennode=p;
						chosentexture=t;
						chosentemp=temp;
						type=3;
					}
				}
				
				b=p->y-t->height;
				for(temp=p->last;temp!=NULL;temp=temp->last)
				{
					if(temp->y<=b)break;
					if(temp->h>p->h)break;
				}
				if(b>=0&&(temp==NULL||temp->y<=b))
				{
					if(limit!=NULL)degree=1.0-(b-limit->y)*1.0/sqrt(t->width*t->height*1.0);
				 	else degree=1.0-b*1.0/sqrt(t->width*t->height*1.0);
					if(degree>MAX)
					{
						MAX=degree;
						chosennode=p;
						chosentexture=t;
						chosentemp=temp;
						type=4;
					}
				}
			}
		}
		
		//each time,after we have choosen the best place and best object to place,
		//we need to update the state of the box.
		//As we have in total 4 situations for a place and an object pair.
		node *temp;	
		switch(type)//According to the type,update the list of places and the list of objects.
		{
			case 1:
				b=chosennode->x+chosentexture->width;
				temp=chosentemp;
				if(temp==NULL)
				{
					int H=end->h;
					end=chosennode;
					end->y=b;
					end->h+=chosentexture->height;
					if(b<W)
					{
						end->next=new node;
						end->next->last=end;
						end=end->next;
						end->x=b;end->y=W;
						end->h=H;
						end->next=NULL;
					}
					else end->next=NULL;
				}
				else
				{
					if(b==temp->x)
					{
						chosennode->y=b;
						chosennode->h+=chosentexture->height;
						chosennode->next=temp;
						temp->last=chosennode;
					}
					else
					{
						int H=temp->last->h;
						chosennode->next=new node;
						chosennode->y=b;
						chosennode->h+=chosentexture->height;
						temp->last=chosennode->next;
						chosennode->next->last=chosennode;
						chosennode->next->next=temp;
						temp=chosennode->next;
						temp->x=b;
						temp->y=temp->next->x;
						temp->h=H;
					}
				}
				if(chosentexture==head)
				{
					head=chosentexture->next;
					if(head!=NULL)head->last=NULL;
				}
				else if(chosentexture==tail)
				{
					tail=chosentexture->last;
					if(tail!=NULL)tail->next=NULL;
				}
				else
				{
					chosentexture->last->next=chosentexture->next;
					chosentexture->next->last=chosentexture->last;
				}
				break;
			case 2:
				
				b=chosennode->x+chosentexture->height;
				temp=chosentemp;
			
				if(temp==NULL)
				{
					int H=end->h;
					end=chosennode;
					end->y=b;
					end->h+=chosentexture->width;
					if(b<W)
					{
						end->next=new node;
						end->next->last=end;
						end=end->next;
						end->x=b;end->y=W;
						end->h=H;end->next=NULL;
					}
					else end->next=NULL;
				}
				else
				{
					
					if(b==temp->x)
					{
						chosennode->y=b;
						chosennode->h+=chosentexture->width;
						chosennode->next=temp;
						temp->last=chosennode;
					}
					else
					{
						int H=temp->last->h;
						chosennode->next=new node;
						chosennode->y=b;
						chosennode->h+=chosentexture->width;
						temp->last=chosennode->next;
						chosennode->next->last=chosennode;
						chosennode->next->next=temp;
						temp=chosennode->next;
						temp->x=b;
						temp->y=temp->next->x;
						temp->h=H;
					}
				}
				
				if(chosentexture==head)
				{
					head=chosentexture->next;
					if(head!=NULL)head->last=NULL;
				}
				else if(chosentexture==tail)
				{
					tail=chosentexture->last;
					if(tail!=NULL)tail->next=NULL;
				}
				else
				{
					chosentexture->last->next=chosentexture->next;
					chosentexture->next->last=chosentexture->last;
				}
				
				break;
			case 3:
				b=chosennode->y-chosentexture->width;
				temp=chosentemp;
				if(temp==NULL)
				{
					int H=begin->h;
					begin=chosennode;
					begin->y=b;
					begin->h+=chosentexture->height;
					if(b<W)
					{
						begin->last=new node;
						begin->last->next=begin;
						begin=begin->last;
						begin->x=b;begin->y=W;
						begin->h=H;
						begin->last=NULL;
					}
					else begin->last=NULL;
				}
				else
				{
					if(b==temp->y)
					{
						chosennode->x=b;
						chosennode->h+=chosentexture->height;
						chosennode->last=temp;
						temp->next=chosennode;
						
					}
					else
					{
						int H=temp->next->h;
						chosennode->last=new node;
						chosennode->x=b;
						chosennode->h+=chosentexture->height;
						temp->next=chosennode->last;
						chosennode->last->next=chosennode;
						chosennode->last->last=temp;
						temp=chosennode->last;
						temp->y=b;
						temp->x=temp->last->y;
						temp->h=H;
					}
				}
				if(chosentexture==head)
				{
					head=chosentexture->next;
					if(head!=NULL)head->last=NULL;
				}
				else if(chosentexture==tail)
				{
					tail=chosentexture->last;
					if(tail!=NULL)tail->next=NULL;
				}
				else
				{
					chosentexture->last->next=chosentexture->next;
					chosentexture->next->last=chosentexture->last;
				}
				break;
			case 4:
				b=chosennode->y-chosentexture->height;
				temp=chosentemp;
				if(temp==NULL)
				{
					int H=begin->h;
					begin=chosennode;
					begin->y=b;
					begin->h+=chosentexture->width;
					if(b<W)
					{
						begin->last=new node;
						begin->last->next=begin;
						begin=begin->last;
						begin->x=b;begin->y=W;
						begin->h=H;begin->last=NULL;
					}
					else begin->last=NULL;
				}
				else
				{
					if(b==temp->y)
					{
						chosennode->x=b;
						chosennode->h+=chosentexture->width;
						chosennode->last=temp;
						temp->next=chosennode;
						
					}
					else
					{
						int H=temp->next->h;
						chosennode->last=new node;
						chosennode->x=b;
						chosennode->h+=chosentexture->width;
						temp->next=chosennode->last;
						chosennode->last->next=chosennode;
						chosennode->last->last=temp;
						temp=chosennode->last;
						temp->y=b;
						temp->x=temp->last->y;
						temp->h=H;
					}
				}
				if(chosentexture==head)
				{
					head=chosentexture->next;
					if(head!=NULL)head->last=NULL;
				}
				else if(chosentexture==tail)
				{
					tail=chosentexture->last;
					if(tail!=NULL)tail->next=NULL;
				}
				else
				{
					chosentexture->last->next=chosentexture->next;
					chosentexture->next->last=chosentexture->last;
				}
				break;
		}
		//Each time we place a new object,print it's information.
		cout<<"*****"<<endl;
			if(type%2==0)swap(chosentexture->height,chosentexture->width);
			cout<<"The"<<chosentexture->id<<"th rectangle : "<<endl;
			cout<<"Left: "<<chosennode->x<<endl;
			cout<<"Right: "<<chosennode->y<<endl; 
			cout<<"Down: "<<chosennode->h-chosentexture->height<<endl;
			cout<<"Up: "<<chosennode->h<<endl;
			cout<<chosennode->x<<" "<<chosennode->y<<endl;
			cout<<chosentexture->width<<" "<<chosentexture->height<<endl;
		cout<<"*****"<<endl;
	}
	//After the objects are all placed,get the upperbound of the height!
	int ans=-999999;
	for(p=begin;p!=NULL;p=p->next)
	{
		ans=max(ans,p->h);
	}
	cout<<ans<<endl;
}
