
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
	for ( int j= 0; j < count; ++j)
		poins[j] = arr[j];
	for(int i = 0; i < sizeof(points);i++){
        cout << points[i] << endl;
    }
	return 0;
}
