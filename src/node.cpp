#include "node.h"
#include <thread>
#include <iostream>
#include <stdexcept>
#include <time.h>
#include <fstream>
#include <algorithm>


using namespace std;

atomic<unsigned> message_count;

unsigned sent_messages() 
{
    return message_count;
}

void reset_sent_messages() 
{
    message_count = 0; 
}

vector<float> LoadFromFile(const string iFileName,
                            int &oNumberOfNode)
{
    ifstream myfile(iFileName);
    vector<float>  oValues;
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
        }

        myfile.close();

    }
    else
    {
        cout << "Unable to open file\n";
    }

    return oValues;
}

void setupNodes(vector<shared_ptr<Node>> & nodes, int & oNumberOfNode, int & oMaxID)
{
    int aNumberOfNode =0;
    vector<float> aValues;

    aValues = LoadFromFile("../input.txt", aNumberOfNode);
    oNumberOfNode = aNumberOfNode;

    cout << "Please choose how to create node id:\n";
    cout << "Input [1] for create ID randomly\n";
    cout << "Input [2] for create ID from 1 to N, then shuffle\n";

    cout << "Input: ";

    int iInput;

    cin >> iInput; 

    nodes.reserve(aNumberOfNode);
    if(iInput == 1)
    {
        srand( (unsigned int) time(0) );

        for(int i=0; i< aNumberOfNode; i++)
        {
            Node temp;    
            int id = rand();

            if(oMaxID < id)
            {
                oMaxID = id;
            }

            nodes.push_back(make_shared<Node>(id));
        }
    }
    else if( iInput == 2)
    {
        
        // create nodes, put them into array
        for (int i = 0; i < aNumberOfNode; ++i)
        {
            nodes.push_back(make_shared<Node>(i + 1));
        }

        // shuffle the nodes randomly
        srand(time(NULL));
        random_shuffle(nodes.begin(), nodes.end());

        oMaxID = aNumberOfNode;
    }

    //set delay values
    for(int i =0; i< aNumberOfNode; i++)
    {
        nodes[i]->setDelay(aValues[i]);
    }

}

static mutex cout_lock;
// helper routine to output full lines from the algorithm
#define atomic_log(x) do { cout_lock.lock(); cout << x; cout_lock.unlock(); } while(0)

Node::Node():   //_participant(false),
                // _transit(false),
                _done(false),
                _portNumber(0),
                _delay(0)
{
}

Node::Node(unsigned node_id) :  _uid(node_id),
                                // _participant(false),
                                // _transit(false),
                                _done(false),
                                _portNumber(0),
                                _delay(0)
{
    if (node_id == 0)
    {
        throw range_error("Node id has to be greater than 0.");
    }
}



void Node::receive_message(Message & message)
{

    std::lock_guard<std::mutex> guard(_message_buffer_lock);
    _message_buffer.push(message);
}


bool Node::checkout_message(Message & message)
{
    // try to lock the message buffer for this thread
    // if it's locked, return false and wait a bit using yield
    if (!_message_buffer_lock.try_lock())
    {
        this_thread::yield();
        return false;
    }

    // if we don't have any messages, yield and return false
    if (_message_buffer.size() == 0)
    {
        _message_buffer_lock.unlock();
        this_thread::yield();
        return false;
    }

    // we have a message, retrieve it, and remove from buffer
    message = _message_buffer.front();
    _message_buffer.pop();

    // don't forget to unlock
    _message_buffer_lock.unlock();
    return true;
}

void Node::connect(shared_ptr<Node> next)
{
    // simple blocking implementation
    // only needed if connecting is done asynchronously
    _next_lock.lock();
    _next = next;
    _next_lock.unlock();
}

void Node::transmit_message(Message message)
{
    shared_ptr<Node> next;
    string res = "";

    // if the node isn't connected yet, do blocking wait
    while(1)
    {
        _next_lock.lock();
        next = _next.lock();
        if (next) break;
        _next_lock.unlock();
        this_thread::yield();
    }

    next->receive_message(message);

    _next_lock.unlock();

    message_count++;
}

void Node::process_message(Message iMessage)
{
    /* detection of leader
*
* - trasmit notification message telling other nodes
* - set self as leader
* - mark algorithm as done
*/
    if(iMessage._type == "election-message")
    {
        if (iMessage._value == _uid)
        {
            atomic_log("Node [ " << _uid << " ] self-declared leader." << endl);
            Message m(_uid, "elected", _uid);
            transmit_message(m);
            _leader.set_value(_uid);
            _done = true;
        }
        else if (iMessage._value > _uid)
        {
            atomic_log("Node [ " << _uid << " ] foward the message ." << iMessage._value << endl);
            Message m(_uid, "election-message", iMessage._value);
            transmit_message(m);

            
        }
        /* if the node is not yet a leader, or in transit mode re-send the base messages */
        else // if (iMessage._value < _uid)
        {
            atomic_log("Node [ " << _uid << " ] join as new candidate." << endl);
            Message m(_uid, "election-message", _uid);
            transmit_message(m);
            
        }
    }
    if(iMessage._type == "elected")
    {
        atomic_log("Node [ " << _uid << " ] recognized node [ " << iMessage._value <<  " ] as the leader node." << endl);
        Message m(_uid, "elected", iMessage._value );
        transmit_message(m);
        _leader.set_value(iMessage._value);
        _done = true;
  
    }


}

bool Node::_start_election = false;

void Node::start_node_logic()
{
    bool first = false;
    if(!_start_election)
    {
        _start_election = true;
        
        Message m(_uid, "election-message", _uid);
        transmit_message(m);
        first = true;
    }
    

    if(first)
    {
        atomic_log("Node [ " << _uid << " ] first candidate." << endl);
        
    }
    Message message1;

    while (true)
    {
        // if the algorithm is done, simply exit the thread
        if (_done)
        {
            return;
        }
        else if (!message1.isEmpty() ) 
        {
            process_message(message1);
            message1.clear();
        }
        // receive new messages
        else
        {
            checkout_message(message1);
        }
        this_thread::yield();
    }
}

bool Node::havePortNumber()
{
    if(_portNumber == 0)
    {
        return false;
    }
    return true;
}

void Node::setPortNumber(const unsigned int & iPortNumber)
{
    _portNumber = iPortNumber;
}

void Node::setDelay(const float & iDelay)
{
    _delay = iDelay;
}

unsigned int Node::getPortNumber()
{
    return _portNumber;
}

void Node::printDebug()
{
    cout << "====================DEBUG LOG====================\n";
    cout << "UID: \t\t" << _uid;
    cout << "\nDelay: \t\t" << _delay;
    cout << "\nPort Number: \t" << _portNumber << endl;
    cout << "=================================================\n";
}


future<int> Node::get_leader()
{
    return _leader.get_future();
}
