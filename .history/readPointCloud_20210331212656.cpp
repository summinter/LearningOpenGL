
#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream> 
using namespace std;
void pcdToXyz(string infilePath, string outfilePath);
void pcdToXyztgb(string infilePath, string outfilePath);
void pcdToImformation(string infilePath, string outfilepath);
using namespace std;

int main()
{
    string infilePath;
    string outfilePath;
    pcdToXyz(".pcd","kk.txt");

}

void pcdToXyz(string infilePath, string outfilePath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilePath);
    string x;
    string y;
    string z;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }
    while (getline(infile, temp))
    {
        infile >> x >> y >> z;
        infile >> temp;
        outfile << x << "\t" << y << "\t" << z << endl;
    }
    infile.close();
    outfile.close();

}


void pcdToXyztgb(string infilePath, string outfilePath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilePath);
    string x;
    string y;
    string z;
    string rgb;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }
    while (getline(infile, temp))
    {
        infile >> x >> y >> z>>rgb;
        
        outfile << x << "\t" << y << "\t" << z <<rgb<< "\t" <<endl;
    }
    infile.close();
    outfile.close();

}


void pcdToImformation(string infilePath, string outfilepath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilepath);
    string x;
    string y;
    string z;
    string rgb;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }
   
    infile.close();
    outfile.close();

}