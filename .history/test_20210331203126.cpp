
#include <iostream>
#include <fstream>
using namespace std;
int verticeNum;
 float* readPointCloud(string name){
    float arr[100000]={0};
    verticeNum = 0;
    ifstream infile(name);
    while(!infile.eof())
    {
        infile>>arr[verticeNum];
        ++verticeNum;
    }
    infile.close();
    float* points = new float[verticeNum];
    for ( int j= 0; j < verticeNum; ++j){
        points[j] = arr[j];
    }
    return points;
}

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
    float* points = new float[count];
	for ( int j= 0; j < count; ++j){
        points[j] = arr[j];
    }
    cout << sizeof(points);
    return 0;
}
