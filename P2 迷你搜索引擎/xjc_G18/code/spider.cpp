#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <set>
#include <queue>
#include <UrlMon.h>
#pragma comment(lib, "urlmon.lib")
using namespace std;
/*
As we use the system function "URLDownloadToFile" to download the website,
wo will meet the situation where we need to transfer string into LPCSWTR.
The function stringToLPCWSTR will do this.
*/
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}
/*

After analysing several pages,we notice that the relationship among the pages in the website can
be simplified as a tree,which means we can use few particular judges to avoid accessing a page again 
rather than using hash or set to make it.

So we define the type URL to memorize a url and it's depth.
the url is devided into 2 part "root"and"next" which will make the implement easier.

*/
struct URL
{
	string root, next;
	int level;
}temp;
queue<URL>que;
/*
    We use BFS to go over each page.
	que is for BFS.
*/

int cnt = 0;
/*
    We simply use the order we access a page as the page's name.
	So we use cnt to record it.
*/
string rt = "http://shakespeare.mit.edu/";
string nt = "index.html";
int lev = 1;
//rt nt and lev is used as a temporary variables.
//initially we let them record the frontpage's information.
string file_root;
string file_name;
//use file_root to record the place we store the pages we download.
//use file_name to record the name of the new page waited to be downloaded.
string getid() //get the name of a new file.
{
	cnt++;
	stringstream  t;
	t<<cnt;
	return (t.str()+".txt");
}
void analysis() //access the page at the front of the BFS que.
{
	rt = temp.root;
	nt = temp.next;
	lev = temp.level;//record the page's information.
    file_name = getid();//get it's name
	string target = rt + nt;  string loc = file_root + file_name;
	//target is the page's address  and  loc is the place we will store it.
	LPCWSTR str1 = stringToLPCWSTR(target);
	LPCWSTR str2 = stringToLPCWSTR(loc);//transfer string into LPCWSTR.
	URLDownloadToFile(NULL, str1, str2, 0, 0);//download the page.

	cout << cnt << " " << target << endl;//print the page's url we download.
	
	if (lev == 3)return;
	/*
	    we notice that the url in the page of depth 3 point
		either it's ancestors or it's brothers.
		no need to analyze it.
	*/
	else
	{
		ifstream infile(loc);
		string TEXT; char t;
		while ((t = infile.get()) != EOF)TEXT = TEXT + t;
		//use a string TEXT to read the page we access.
		const char *pos = TEXT.c_str();//define a pointer of it.
		while (pos != NULL)//capture the url in this page.
		{
			pos = strstr(pos, "href=\"");//find the next url.
			if (pos != NULL)//if we found it,then get it.
			{
				pos += strlen("href=\"");
				//We notice that the url begins with "http" would point outside of 
				//the website.No need to get them.
				if (*pos == '/'||(*pos == 'h'&&*(pos+1)=='t'&&*(pos+2)=='t'&&*(pos+3)=='p'))continue;
				else
				{
					temp.root=rt;
					temp.next.clear();
					while (*pos != '/' && *pos != '\"')
					{
						temp.root = temp.root + (*pos);
						pos++;
						
					}//get the url's first part;
					if (lev == 1)
					{
						/*
						  as only the page in the level 1 and 2 will have
						  a the second part,so only when we capture the url in
						  level 1 we need to get it.
						*/
						if (*pos == '/')
						{
							temp.root = temp.root + '/';
							pos++;
							while (*pos != '\"')
							{
								temp.next = temp.next + (*pos);
								pos++;
							}
						}
					}
					//if (cnt == 39){ cout << "aaaaaaaaaaaaaaaaaaaaaa!!!!!!!!!!!" << endl; }
					temp.level = lev + 1;
					que.push(temp);//insert the url captured into the queue.
				}
			}

		}
		pos = TEXT.c_str();
		while (pos != NULL)//capture the url in this page.
		{
			pos = strstr(pos, "HREF=\"");//find the next url.
			if (pos != NULL)//if we found it,then get it.
			{
				pos += strlen("HREF=\"");
				//We notice that the url begins with "http" would point outside of 
				//the website.No need to get them.
				if (*pos == '/' || (*pos == 'h'&&*(pos + 1) == 't'&&*(pos + 2) == 't'&&*(pos + 3) == 'p'))continue;
				else
				{
					temp.root = rt;
					temp.next.clear();
					while (*pos != '/' && *pos != '\"')
					{
						temp.root = temp.root + (*pos);
						pos++;

					}//get the url's first part;
					if (lev == 1)
					{
						/*
						as only the page in the level 1 and 2 will have
						a the second part,so only when we capture the url in
						level 1 we need to get it.
						*/
						if (*pos == '/')
						{
							temp.root = temp.root + '/';
							pos++;
							while (*pos != '\"')
							{
								temp.next = temp.next + (*pos);
								pos++;
							}
						}
					}
					//if (cnt == 39){ cout << "aaaaaaaaaaaaaaaaaaaaaa!!!!!!!!!!!" << endl; }
					temp.level = lev + 1;
					que.push(temp);//insert the url captured into the queue.
				}
			}


		}
	}
}
int main()
{
	CreateDirectory(stringToLPCWSTR("./html"), 0);//create a directory for the pages.

	file_root = ("./html//");
	temp.root = rt;
	temp.next = nt;
	temp.level = lev;
	
	que.push(temp);//push the frontpage into the queue.
	while (!que.empty())//BFS.
	{
		temp = que.front();
		que.pop();
		analysis();
	}
	getchar();
	return 0;
}