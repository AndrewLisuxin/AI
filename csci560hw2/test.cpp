#include<iostream>
#include<limits>
#define MIN numeric_limits<int>::min()
using namespace std;
int main()
{
	int x = 0;
	if(!x)
		cout << "hello!" << endl;

	cout<< numeric_limits<int>::max() <<endl;
	int a = MIN;
	cout<< a <<endl;
	
	return 0;
}
