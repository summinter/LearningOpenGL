
#include <iostream>
#include <fstream>
using namespace std;
 
 
int main()
{   
	float arr[100000]={0};
	ifstream infile("milk.txt");
	int count = 0;
	while(!infile.eof())
	{
		infile>>arr[count];
		++count;
	}
	infile.close();
    cout << count;
    float* points = new float[count];
	for ( int j= 0; j < count; ++j){
        points[j] = arr[j];
    }
    cout << *(points - 4);
    return 0;
}
