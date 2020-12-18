#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
// #include <boost/json.hpp>

using namespace std;
// using json11 as Json;

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
            int val) : _source_UID(id), _type(type), _value(val), _isEmpty(false)
    {

    }

    bool isEmpty()
    {
        if(_source_UID ==0)
        {
            return true;
        }

        return false;
    }

    void clear()
    {
        _isEmpty = true;
        _source_UID = 0;
        _type = "";
        _value = 0;
    }

    void printDebug()
    {
        
        cout <<"_source_UID: "<< _source_UID;
        cout <<"\n_type: " << _type;
        cout << "\n_value: " << _value << endl;
    }

    string debugString()
    {
        string res = "";
        res += ("_source_UID: " + to_string(_source_UID));
        res += ("\n_type: " + _type);
        res += ("\n_value: " + to_string(_value));
        res += "\n";
        return res;
    }
    

};


#endif // MESSAGE_H