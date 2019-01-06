#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#define alpah 1e-9
using namespace std;
const double inf=999999999;
double ans=inf;
struct node
{
	double x,y;
	int id;
	friend double operator * (node a,node b)
	{
		return a.x*b.y-a.y*b.x;
	}
	friend node operator + (node a,node b)
	{
		return node{a.x+b.x,a.y+b.y};
	}
	friend node operator - (node a,node b)
	{
		return node{a.x-b.x,a.y-b.y};
	}
}P[305];
int N,n;
double S[305][305];
double f[305][15];
int fz[305][15];
int cnt;int choose[305];
void qsort(int l,int r)
{
	int x,y;node temp,mid;
	x=l;y=r;mid=(P[(l+r)/2]-P[1]);
	while(x<=y)
	{
		while((P[x]-P[1])*mid>0)x++;
		while((P[y]-P[1])*mid<0)y--;
		if(x<=y)
		{
			temp=P[x];P[x]=P[y];P[y]=temp;
			x++;y--;
		}
	}
	if(l<y)qsort(l,y);
	if(x<r)qsort(x,r);
}
int main()
{
	int a,b,c,d,e;
	scanf("%d%d",&N,&n);
	for(a=1;a<=N;a++)
	{
		scanf("%lf%lf",&P[a].x,&P[a].y);
		P[a].id=a-1;
	}
	qsort(2,N);
	for(a=1;a<=N;a++)
	{
		S[a][a]=0;S[a][a+1]=0;
		for(b=a+2;b<=N;b++)S[a][b]=S[a][b-1]+((P[b-1]-P[a])*(P[b]-P[a]))*0.5;
	}
	for(a=1;a<=N;a++)for(b=a-1;b>=1;b--)S[a][b]=S[1][N]-S[b][a];	
	for(a=1;a<=N;a++)
	{
		for(b=0;b<=N;b++)for(c=0;c<=n;c++)f[b][c]=inf;
		f[a][1]=0;
		for(b=2;b<n;b++)
		{
			e=N-(n-b);
			for(c=a+b-1;c<=e;c++)
			for(d=a+b-2;d<c;d++)
			{
				if(f[c][b]>f[d][b-1]+S[d][c])
				{
					f[c][b]=f[d][b-1]+S[d][c];
					fz[c][b]=d;
				}
			}
		}
		int FROM=-1;
		for(c=a+n-1;c<=N;c++)
		{
			for(d=a+n-2;d<c;d++)
			{
				if(f[c][n]>f[d][n-1]+S[d][c]+S[c][a])
				{
					f[c][n]=f[d][n-1]+S[d][c]+S[c][a];
					fz[c][n]=d;
				}
			}
			if(f[c][n]<ans)
			{
				ans=f[c][n];
				FROM=c;
			}
		}
		if(FROM!=-1)
		{
			cnt=0;
			while(FROM!=a)
			{
				cnt++;
				choose[cnt]=FROM;
				FROM=fz[FROM][n-cnt+1];
			}
			cnt++;
			choose[cnt]=a;
		}
	}
	for(a=1;a<cnt;a++)cout<<P[choose[a]].id<<" ";
	cout<<choose[cnt]-1;
	return 0;
}
