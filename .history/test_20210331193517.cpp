#include <iostream>
#include <fstream>
using namespace std;

float* readPointCloud(string fileName){
    ifstream inFile;
    int count;
    float* points = new float[count];
    inFile.open(fileName);
    if(inFile.is_open()){

    }
}

int main(){
    string fileName = "milk.txt";
    float* points = readPointCloud(fileName);
    cout << poin
    return 0;
}
