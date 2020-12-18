#ifndef NODE_H
#define NODE_H

#include <queue>
#include <mutex>
#include <memory>
#include <future>
#include <atomic>
#include <string>
#include <vector>
#include "message.h"

class Node
{
    

public:
    Node();
    Node(unsigned node_id);

    /** \brief Put a new message into this nodes message buffer */
    // void receive_message(int node_id, int distance);
    void receive_message(Message &message);
    /** \brief Put a new message into this nodes message buffer */
    // void receive_message(const std::pair<int,int>& message);

    /** \brief Connect this node to a next node in the circle */
    void connect(std::shared_ptr<Node> next);

    /** \brief Node logic */
    void start_node_logic( );

    /** \brief Get the leader node id */
    std::future<int> get_leader();

    bool havePortNumber();
    void setPortNumber(const unsigned int & iPortNumber);
    unsigned int getPortNumber();

    void setDelay(const float & iDelay);
    
    void printDebug();

    static bool _start_election ;

private:
    /** \brief Try to pull one message from the buffer
*
* Non-blocking operation.
*/
    bool checkout_message(Message & message);

    /** \brief Transmit one message to the connected node
*
* Blocking operation.
*/
    void transmit_message(Message message);

    /** \brief Sub logic for processing the message
*
* Process the received message
* - detection of leader
* - detection of transiting state
* - trasmition of appropriate messages
*/
    void process_message(Message iMessage);


private:
    std::queue<Message> _message_buffer;
    std::mutex _message_buffer_lock;

    std::weak_ptr<Node> _next;
    std::mutex _next_lock;

    
    unsigned int _portNumber;
    float _delay;

    int _uid;
    bool _done;
    std::promise<int> _leader;
};

extern std::atomic<unsigned> message_count;

unsigned sent_messages();

void reset_sent_messages();

std::vector<float> LoadFromFile(const std::string iFileName,
                                int &oNumberOfNode);

void setupNodes(std::vector<std::shared_ptr<Node>> & nodes, int & oNumberOfNode, int & oMaxID);


#endif // NODE_H