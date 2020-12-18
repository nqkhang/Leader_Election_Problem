#include <iostream>
#include "node.h"
#include <thread>
#include <stdexcept>
#include <unistd.h>

using namespace std;

int main()
{
    // initialization
    reset_sent_messages();

    // prepare the array
    int aNumberOfNode = 0;
    int aMaxID =0;
    vector<shared_ptr<Node>> nodes;

    setupNodes(nodes, aNumberOfNode, aMaxID);

    // connect the nodes
    for (unsigned i = 0; i < aNumberOfNode - 1; ++i)
    {
        nodes[i]->connect(nodes[i + 1]);
    }

    nodes[aNumberOfNode - 1]->connect(nodes[0]);

    // print node info
    cout << "\nNodes created:\n";
    for (auto i: nodes)
    {
        i->printDebug();
    }
    cout << endl;

    cout << "\n\nPress any key to start the election...";
    string dummy;
    cin >> dummy;

    // prepare the futures to store the elected leader information
    vector<future<int>> leaders;
    for (auto i : nodes)
    {
        leaders.push_back(i->get_leader());
    }
    cout << endl;

    // run the threads with the main node logic
    for (auto i : nodes)
    {
        thread([](shared_ptr<Node> node) {  node->start_node_logic( ); }, i).detach();
    }


    // do the final synchronization (so that main doesn't end before the algorithm does)
    for (unsigned i = 0; i < aNumberOfNode; i++)
    {
        if (leaders[i].get() != (int)aMaxID) //make sure every node have acknowledge the leader
        {
            throw runtime_error("Node didn't correctly detect it's leader.");
        }
    }

    // final reports
    cout << "\nAsynchronous run finished." << endl;
    cout << "Total number of sent messages was : " << sent_messages() << endl;

    return 0;
}