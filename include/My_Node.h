#include <iostream>
using namespace std;

class My_Node
{
private:
    unsigned int    _uid = 0;
    unsigned int    _portNumber = 0;
    float           _delay = 0;

    unsigned int generateUID();


public:
    My_Node();
    My_Node(const float & iDelay);
    ~My_Node();

    void printDebug();
};