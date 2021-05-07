
#include <iostream>
#include <fstream>
using namespace std;
 
 
int _tmain(int argc, _TCHAR* argv[])
{
	float arr[100000]={0};
	ifstream infile("kk.txt");
	int count = 0;
	while(!infile.eof())
	{
		infile>>arr[count];
		++count;
	}
	infile.close();
 
	for ( int j= 0; j < count; ++j)
		cout<<arr[j]<<endl;
	
	return 0;
}
