#define note (x->ls->height-x->rs->height)  //use note to calculate the balanced factor of x;
#define notee (y->ls->height-y->rs->height) //use notee to calculate the balanced factor of y; 
struct AVL
{
	int height;
	AVL *ls,*rs,*fz;
	string word,filename;
	int lineNum, offset;
	Splay(Splay *father,string _word,string _name,string _set)):ls(0),rs(0),fz(father),size(1),word(_word),filename(_name),fileOffset(_set){};
	AVL(string _word,string _name,int _num,int _set,int h,AVL *f,AVL *l,AVL *r):
	word(_word),filename(_name),lineNum(_num),offset(_set),height(h),fz(f),ls(l),rs(r){};//initialization
}*_none=new AVL("","","",-1,0,0,0),*root=_none;//define the AVL's mumber.
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
void _insert(string _word,string _name,int _num,int _set)
{
	if(root==_none)// if the tree is empty, just make the inserted node the root;
	{
		root=new AVL(_word,_name,_num,_set,0,_none,_none,_none);
		return;
	}
	else
	{
		//search the inserting position from root.
		AVL *p=root;AVL *y;
		while(p!=_none)
		{
			if(_word<p->word)//if x is lesser than the point we are now in ,go left.
			{
				if(p->ls!=_none)p=p->ls;//we haven't get the position,just go
				else
				{
					//we have arrived ,insert it.
					p->ls=y=new AVL(_word,_name,_num,_set,0,p,_none,_none);
					break;
				}
			}
			else //x is greater than the point we arrived, go right
			{
				//the same to the situation above
				if(p->rs!=_none)p=p->rs;
			    else
			    {
			    	p->rs=y=new AVL(_word,_name,_num,_set,0,p,_none,_none);
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
void _del(string _word)//delete the element val.
{
	AVL *p=root;
	//start from the root to search our point.
	while(p!=_none)
	{
		if(p->word==_word)//have found
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
					p->word=t->word;
					p->filename=t->filename;
					p->lineNum = p->lineNum;
					p->offset = p->offset;
					_word=t->word;p=p->ls;
				}
				else
				{
					AVL *t=getmin(p->rs);
					p->word=t->word;
					p->filename=t->filename;
					p->lineNum = p->lineNum;
					p->offset = p->offset;
					_word=t->word;p=p->rs;
				}
			}
		}
		else//haven't found yet;
		{  
		    //search along the tree.
		    if(_word>p->word)p=p->rs;
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
AVL *find(string x) {
	AVL *p = root;
	while (p)
	{
		if(p->word==x)break;
    	if(x<p->word)p=p->ls;
    	else p=p->rs;
	}
	return p;
}
