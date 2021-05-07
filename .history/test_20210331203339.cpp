
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
	float *vertices = readPointCloud("milk.txt");
    for (int i = 0; i <verticeNum;i++){
        // cout << vertices[i] << endl;
    }

       float vertices2[] = {
           0.5, 0.5 ,0,
           0.1, 0.2, 0,
           0.8, 0.3, 0,
           0.3, 0.3, 0.3
   };
       for (int i = 0; i <verticeNum;i++){
        cout << vertices2[i] << endl;
    }


}
