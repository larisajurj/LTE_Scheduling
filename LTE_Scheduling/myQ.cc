#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
    queueLength = par("qlp");
}

void MyQ::setQlp(int value) {
    queueLength = value; // Update the runtime-modifiable qlp
}

int MyQ::getQlp() const {
    return queueLength; // Return the current value of the runtime-modifiable qlp
}


void MyQ::handleMessage(cMessage *msg)

{

    int ql = 0;
    int i =  getParentModule()->getIndex();

    if (msg->arrivedOn("rxPackets")){
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        //read parameters from msg
        int nrOfRadioBlocks = (int)msg->par("nrBlocks");
        delete msg;
        //empty the queue !
        for(int j = 0; j < nrOfRadioBlocks && !queue.isEmpty(); j++ ){
           EV << "Sending " << nrOfRadioBlocks<< " radio blocks for user " << i << endl;
           msg = (cMessage *)queue.pop();
          send(msg, "txPackets");
        }
    }
    ql = queue.getLength();
    setQlp(ql);
}
