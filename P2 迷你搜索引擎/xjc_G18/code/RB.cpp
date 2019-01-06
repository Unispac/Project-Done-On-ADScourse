const int black=0;
const int red=1;
struct RBtree
{
	int color,size;
	string word, filename;
	int lineNum;
	int Offset;
	RBtree *ls,*rs,*fz;
	RBtree(int hue,string _word,string _name,int _num,int _set,RBtree *anc,RBtree *l,RBtree *r,int s):
	color(hue),word(_word),filename(_name),lineNum(_num),Offset(_set),fz(anc),ls(l),rs(r),size(s){};
}*nil=new RBtree(black,"","","",0,0,0,0),*root=nil;
void zig(RBtree *x)
{
	RBtree *y,*z;
	y=x->fz;z=y->fz;
	if(y==z->ls)z->ls=x;else z->rs=x;
	x->fz=z;y->ls=x->rs;y->ls->fz=y;
	x->rs=y;y->fz=x;
	if(x->fz==nil)root=x;
	x->size=y->size;
	y->size=y->ls->size+y->rs->size+1;
	return;
}
void zag(RBtree *x)
{
	RBtree *y,*z;
	y=x->fz;z=y->fz;
	if(y==z->ls)z->ls=x;else z->rs=x;
	x->fz=z;y->rs=x->ls;y->rs->fz=y;
	x->ls=y;y->fz=x;
	if(x->fz==nil)root=x;
	x->size=y->size;
	y->size=y->ls->size+y->rs->size+1;
	return;
}
void _insert_fix(RBtree *x)
{
	if(x==root){x->color=black;return;}
	if(x->fz->color==black)return;
	RBtree *p,*g,*u;
	p=x->fz;g=p->fz;
	if(p==g->ls)u=g->rs;else u=g->ls;
	if(u->color==red)
	{
		u->color=black;
		p->color=black;
		g->color=red;
		_insert_fix(g);
	    return;
	}
	else
	{
		if(p==g->ls)
		{
			if(x==p->ls){p->color=black;g->color=red;zig(p);}
			else {x->color=black;g->color=red;zag(x);zig(x);}
		} 
		else
		{
			if(x==p->rs){p->color=black;g->color=red;zag(p);}
			else {x->color=black;g->color=red;zig(x);zag(x);}
		}
		return;
	}
}
void _insert(string _word,string _name,string _set)
{
	if(root==nil)
	{
		root=new RBtree(black,_word,_name,_num,_set,nil,nil,nil,1);
		return;
	}
	else
	{
		RBtree *p=root;
		while(p!=nil)
		{
			p->size++;
			if(_word>p->word)
			{
				if(p->rs!=nil)p=p->rs;
				else
				{
					p->rs=new RBtree(red,_word,_name,_num,_set,p,nil,nil,1);
					p=p->rs;
					break;
				}
			}
			else
			{
				if(p->ls!=nil)p=p->ls;
				else
				{
					p->ls=new RBtree(red,_word,_name,_num,_set,p,nil,nil,1);
					p=p->ls;
					break;
				}
			}
		}
		_insert_fix(p);
	}
	return;
}
RBtree *getmin(RBtree *p)
{
	while(p->ls!=nil)p=p->ls;
	return p;
}
void _del_fix(RBtree *x)
{
	if(x==root)
	{
		x->color=black;
		return;
	}
	RBtree *y,*z;
	if(x->color==red){x->color=black;return;}
	y=x->fz;
	if(x==y->ls)
	{
		z=y->rs;
		if(z->color==red)
		{
			z->color=black;y->color=red;
			zag(z);_del_fix(x);return;
	    }
	    else 
	    {
	    	if(z->ls->color==black&&z->rs->color==black)
	    	{
	    		z->color=red;_del_fix(y); return;
			}
			else if(z->rs->color==black) 
			{
				z->color=red;
				z->ls->color=black;
				zig(z->ls);_del_fix(x);return;
			}
			else 
			{
				z->rs->color=z->color;
				z->color=y->color;
				y->color=black;
				zag(z);
				return;
			}
	    }
	}
	else
	{
		z=y->ls;
		if(z->color==red)
		{
			z->color=black;y->color=red;
			zig(z);_del_fix(x);return;
		}
		else
		{
			if(z->ls->color==black&&z->rs->color==black)
			{
				z->color=red;_del_fix(y);return;
			}
			else if(z->ls->color==black)
			{
				z->rs->color=black;z->color=red;zag(z->rs);
				_del_fix(x);return;
			}
			else
			{
				z->ls->color=z->color;z->color=y->color;y->color=black;
				zig(z);return;
			}
		}
	}
}
void _del(string _word)
{
	RBtree *p=root;
	while(p!=nil)
	{
		if(p->word==_word)
		{
			if(p->ls==nil&&p->rs==nil)
			{
				if(p->color==red){if(p->fz->ls==p)p->fz->ls=nil;else p->fz->rs=nil;delete p;p=0;break;}
	            else break;
			}
			else if(p->rs==nil)
			{
				p->word=p->ls->word;
				p->filename=p->ls->filename;
				p->lineNum = p->lineNum;
				p->Offset = p->Offset
				delete p->ls;
				p->ls=nil;
				p=0;
				break;
			}
			else if(p->ls==nil)
			{
				p->word=p->rs->word;
				p->filename=p->rs->filename;
				p->lineNum = p->lineNum;
				p->Offset = p->Offset;
			    delete p->rs;
			    p->rs=nil;
			    p=0;
			    break;
			}
			else
			{
				RBtree *t=getmin(p->rs);
				_word=p->word=t->word;
				p->filename=t->filename;
				p->lineNum = p->lineNum;
				p->Offset = p->Offset;
				p=p->rs;
			}
		}
		else
		{
			if(_word>p->word)p=p->rs;
			else p=p->ls;
		}
	}
	if(p!=0)
	{
	    _del_fix(p);
	    if(p->fz->ls==p)p->fz->ls=nil;else p->fz->rs=nil;
		delete p;
    }
	return;
}
RBtree *find(string x) {
	RBtree *p = root;
	while (p)
	{
		if (p->word == x)break;
		if (x<p->word)p = p->ls;
		else p = p->rs;
	}
	return p;
}