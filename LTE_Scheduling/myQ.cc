#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

void MyQ::handleMessage(cMessage *msg)

{

    ql = 0; //=9;
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
    EV << "ql[" << i<<"] = " << ql << endl;

}
