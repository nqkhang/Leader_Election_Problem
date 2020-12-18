#include "message.h"

// message::ImportFromJSONString()

bool Message::isEmpty()
{
    if(_source_UID ==0)
    {
        return true;
    }
    return false;
}

void Message::clear()
{
    _isEmpty = true;
    _source_UID = 0;
    _type = "";
    _value = 0;
}

void Message::printDebug()
{
    
    cout <<"_source_UID: "<< _source_UID;
    cout <<"\n_type: " << _type;
    cout << "\n_value: " << _value << endl;
}

string Message::debugString()
{
    string res = "";
    res += ("_source_UID: " + to_string(_source_UID));
    res += ("\n_type: " + _type);
    res += ("\n_value: " + to_string(_value));
    res += "\n";
    return res;
}