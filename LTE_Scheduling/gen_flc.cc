#include "gen_flc.h"

Define_Module( GeneratorFLC );

#define ev EV
using namespace omnetpp;

void GeneratorFLC::initialize()
{  
	flc_time = 1;


	//(simtime_t)getParentModule()->par("flc_time");;
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(0, sendMessageEvent);
}
   

void GeneratorFLC::handleMessage(cMessage *msg)
{   
	simtime_t next_time;

    ASSERT(msg==sendMessageEvent);
	ev<<" START FLC\n";
    cMessage *msgr = new cMessage("start_flc"); 
    send(msgr,"out");
    next_time = simTime()+ flc_time;
    scheduleAt(next_time,sendMessageEvent);
}

void GeneratorFLC::finish()
{
    ev << "*** Module: " << getFullPath() << "***" << endl;
}
