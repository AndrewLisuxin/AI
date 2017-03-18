#include<vector>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<unordered_map>
#include<list>
#include<unordered_set>
using namespace std;
void add(vector<pair<pair<int,int>,int>> p);
typedef struct{
	string name;
	int age;
	string address;
	
}People;
typedef pair<string, vector<int>> HH;

HH hh(HH a);
int main()
{
vector<int> a = {1,2,3};

vector<int> & b = a;

b[0] = 0;

cout << a[0] << endl;
cout << b[0] << endl;

add(vector<pair<pair<int,int>,int>>{pair<pair<int,int>,int>(pair<int,int>(1,2),3)});

People *ap = new People; ap->name = "Jim";

People *bp = new People; bp->name = "Andy";


list<People *>list1 = {ap,bp};
list<People *>list2 = {ap,bp};
list1.erase(list1.begin());
cout << (*list1.begin())->name << endl;
cout << (*list2.begin())->name << endl;

list<int> c = {4,3,2};
list<int> d = {1,4,7};
c.insert(c.end(), d.begin(),d.end());
for(list<int>::iterator it = c.begin(); it != c.end(); it ++)
	cout << *it<< "  ";
cout<<endl;
for(list<int>::iterator it = d.begin(); it != d.end(); it ++)
	cout << *it<< "  ";
HH aa = pair<string, vector<int>>("lisuixn", vector<int>{1,2,3});

HH bb = hh(aa);

cout << aa.first<<endl;
cout << bb.first << endl;
//unordered_set<pair<string,string>> x;
//x.insert(pair<string,string>("hello","hi"));

//unordered_set<string> y;
//y.insert("Jim.x1");
return 0;
}

HH hh(HH a)
{ 
        HH aa = a;
	aa.first = "Jack";
	return aa;
}
void add(vector<pair<pair<int,int>,int>> p)
{
	
	p[0].first.second -= 7;
cout << p[0].first.second << endl;

}
