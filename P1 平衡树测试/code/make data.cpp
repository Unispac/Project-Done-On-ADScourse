#include<algorithm>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<ctime>
using namespace std;
int P[10005];//store the data we make
bool mark[10000000];//As we want to make distinct number,we need mark them. 
inline void  getnum(int &c)//create a random number from 0 to 9999999
{
	c=rand()%10*1000000+rand()%10*100000+rand()%10*10000+rand()%10*1000+rand()%10*100+rand()%10*10+rand()%10;
	return;
}
void rdsort(int *P,int n)//random sort,make the order randomed.
{
	int a,b,c;
	for(a=1;a<=n;a++)
	{
		b=rand()%n+1;
		c=P[a];P[a]=P[b];
		P[b]=c;//random swap.
	}
	return;
}
int main()
{
	
    ofstream t1_o1("type1_ordered1.in");ofstream t1_o2("type1_ordered2.in");ofstream t1_r("type1_randomed.in");
    ofstream t2_o1("type2_ordered1.in");ofstream t2_o2("type2_ordered2.in");ofstream t2_r("type2_randomed.in");
    ofstream t3_o1("type3_ordered1.in");ofstream t3_o2("type3_ordered2.in");ofstream t3_r("type3_randomed.in");
    ofstream t4_o1("type4_ordered1.in");ofstream t4_o2("type4_ordered2.in");ofstream t4_r("type4_randomed.in");
    /*
        As we have in total 4 types and 3 orders.
        The data we make will be distributed to 12 files.
    */
    srand((unsigned)time(0));//use time as the seed.
	int a,b,c,d,e;
	int T=1000;int n;//T denotes the quantity of groups and n denotes the size of the data. 
//____________________________________________________________________________________________________________
//Three orders of the type n=1000;	
	t1_o1<<T<<endl;t1_o2<<T<<endl;t1_r<<T<<endl;//output the quantity of groups.
	n=1000;//define n =1000;
	for(a=1;a<=T;a++)
	{
	    t1_o1<<n<<" "<<n<<endl;t1_o2<<n<<" "<<n<<endl;t1_r<<n<<" "<<n<<endl;
        //output the size of insertion and deletion
		for(b=1;b<=n;b++)
		{
			getnum(c);while(mark[c]==true)getnum(c);//make dinstict random number
			mark[c]=true;//mark the number
			P[b]=c;t1_r<<P[b]<<" ";//store the random sequence and output the random sequence
		}
		t1_r<<endl;sort(P+1,P+n+1);//sort the sequence in increasing order.
		for(b=1;b<=n;b++)t1_o1<<P[b]<<" ";t1_o1<<endl;
		for(b=1;b<=n;b++)t1_o1<<P[b]<<" ";t1_o1<<endl;
		for(b=1;b<=n;b++)t1_o2<<P[b]<<" ";t1_o2<<endl;
		for(b=n;b>=1;b--)t1_o2<<P[b]<<" ";t1_o2<<endl;//output the ordered sequence
        rdsort(P,n);//upset the order
		for(b=1;b<=n;b++)t1_r<<P[b]<<" ";t1_r<<endl;//output
		for(b=1;b<=n;b++)mark[P[b]]=false;//restore the mark
	}
//______________________________________________________________________________________________
//three orders of the type n=3000
	t2_o1<<T<<endl;
	t2_o2<<T<<endl;
	t2_r<<T<<endl;
	n=3000;
	for(a=1;a<=T;a++)
	{

	    t2_o1<<n<<" "<<n<<endl;
	    t2_o2<<n<<" "<<n<<endl;
		t2_r<<n<<" "<<n<<endl;
		for(b=1;b<=n;b++)
		{
			getnum(c);
			while(mark[c]==true)getnum(c);
			mark[c]=true;
			P[b]=c;t2_r<<P[b]<<" ";
		}
		t2_r<<endl;
		sort(P+1,P+n+1);
		for(b=1;b<=n;b++)t2_o1<<P[b]<<" ";t2_o1<<endl;
		for(b=1;b<=n;b++)t2_o1<<P[b]<<" ";t2_o1<<endl;
		for(b=1;b<=n;b++)t2_o2<<P[b]<<" ";t2_o2<<endl;
		for(b=n;b>=1;b--)t2_o2<<P[b]<<" ";t2_o2<<endl;
        rdsort(P,n);
		for(b=1;b<=n;b++)t2_r<<P[b]<<" ";t2_r<<endl;
		for(b=1;b<=n;b++)mark[P[b]]=false;
	}
//______________________________________________________________________________________________
//three orders of the type n=7000
	t3_o1<<T<<endl;
	t3_o2<<T<<endl;
	t3_r<<T<<endl;
	n=7000;
	for(a=1;a<=T;a++)
	{

	    t3_o1<<n<<" "<<n<<endl;
	    t3_o2<<n<<" "<<n<<endl;
		t3_r<<n<<" "<<n<<endl;
		for(b=1;b<=n;b++)
		{
			getnum(c);
			while(mark[c]==true)getnum(c);
			mark[c]=true;
			P[b]=c;t3_r<<P[b]<<" ";
		}
		t3_r<<endl;
		sort(P+1,P+n+1);
		for(b=1;b<=n;b++)t3_o1<<P[b]<<" ";t3_o1<<endl;
		for(b=1;b<=n;b++)t3_o1<<P[b]<<" ";t3_o1<<endl;
		for(b=1;b<=n;b++)t3_o2<<P[b]<<" ";t3_o2<<endl;
		for(b=n;b>=1;b--)t3_o2<<P[b]<<" ";t3_o2<<endl;
        rdsort(P,n);
		for(b=1;b<=n;b++)t3_r<<P[b]<<" ";t3_r<<endl;
		for(b=1;b<=n;b++)mark[P[b]]=false;
	}
//______________________________________________________________________________________________
//three orders of the type n=10000
	t4_o1<<T<<endl;
	t4_o2<<T<<endl;
	t4_r<<T<<endl;
	n=10000;
	for(a=1;a<=T;a++)
	{
		
	    t4_o1<<n<<" "<<n<<endl;
	    t4_o2<<n<<" "<<n<<endl;
		t4_r<<n<<" "<<n<<endl;
		for(b=1;b<=n;b++)
		{
			getnum(c);
			while(mark[c]==true)getnum(c);
			mark[c]=true;
			P[b]=c;t4_r<<P[b]<<" ";
		}
		t4_r<<endl;
		sort(P+1,P+n+1);
		for(b=1;b<=n;b++)t4_o1<<P[b]<<" ";t4_o1<<endl;
		for(b=1;b<=n;b++)t4_o1<<P[b]<<" ";t4_o1<<endl;
		for(b=1;b<=n;b++)t4_o2<<P[b]<<" ";t4_o2<<endl;
		for(b=n;b>=1;b--)t4_o2<<P[b]<<" ";t4_o2<<endl;
        rdsort(P,n);
		for(b=1;b<=n;b++)t4_r<<P[b]<<" ";t4_r<<endl;
		for(b=1;b<=n;b++)mark[P[b]]=false;
	}
	return 0;
}
