#include "My_Node.h"
#include <iostream>
#include <time.h>

using std::cout;
using std::endl;
using std::string;


My_Node::My_Node()
{
    _uid = generateUID();
}

My_Node::My_Node(const float & iDelay)
{
    _uid = generateUID();
    _delay = iDelay;
}

My_Node::~My_Node()
{

}

unsigned int My_Node::generateUID()
{
    srand((unsigned int) time(0));

    return rand();
}

void My_Node::printDebug()
{
    cout << "====================DEBUG LOG====================\n";
    cout << "UID: \t\t" << _uid;
    cout << "\nDelay: \t\t" << _delay;
    cout << "\nPort Number: \t" << _portNumber << endl;
    cout << "=================================================\n";
}