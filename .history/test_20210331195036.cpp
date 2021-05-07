#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

float* readPointCloud(string fileName){
    int count = 0;
    float* points[1000000] = {0};
    ifstream infile("kk.txt");
	while(!infile.eof())
	{
		infile>>points[count];
		++count;
	}
	infile.close();

}

int main(){
    string fileName = "milk.txt";
    float* points = readPointCloud(fileName);
    cout << poin
    return 0;
}
