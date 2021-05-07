
#include <iostream>
#include <fstream>
using namespace std;
 
 
int main()
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
    float* points = new float(count);
	for ( int j= 0; j < count; ++j){
        // cout << arr[j] << endl;
        points[j] = arr[j];
    }
	for ( int j= 0; j < count; ++j){
        cout << points[j] << endl;
    }
    return 0;
}
