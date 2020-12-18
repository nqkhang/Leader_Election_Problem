#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "My_Node.h"

using namespace std;
// using std::cout;
// using std::cin;
// using std::vector;
// using std::string;

void RunTest()
{
    My_Node a(3.14);

    a.printDebug();
}

void LoadFromFile(const string iFileName,
                  int &oNumberOfNode,
                  vector<float> oValues)
{
    ifstream myfile(iFileName);
    if (myfile.is_open())
    {
        string line;
        getline(myfile, line);

        oNumberOfNode = stoi(line);
        
        for (int i =0; i< oNumberOfNode; i++)
        {
            getline(myfile, line);
            float temp = stof(line);
            oValues.push_back(temp);
            // cout << temp << endl;
        }

        myfile.close();
    }
    else
    {
        cout << "Unable to open file\n";
    }
}

int main()
{
    RunTest();

    vector<My_Node> nodes;

    int aNumberOfNode =0;
    vector<float> arrValue;
    LoadFromFile("../input.txt", aNumberOfNode, arrValue );


    for (float val : arrValue)
    {
        //create new threads and pass the value here!
    }

    return 0;
}

