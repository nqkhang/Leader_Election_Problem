#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>

using namespace std;

struct Message
{
    int _source_UID =0;
    string _type;
    int _value;
    int _isEmpty =true;

    Message()
    {
    }

    Message(int id,
            string type,
            int val) :  _source_UID(id),
                        _type(type),
                        _value(val),
                        _isEmpty(false)
    {
    }

    bool isEmpty();
    void clear();

    void printDebug();
    string debugString();
};


#endif // MESSAGE_H