#include <omnetpp.h>

using namespace omnetpp;

class GeneratorFLC : public cSimpleModule
{
   private:
	cMessage *sendMessageEvent;
	
    protected: 
	simtime_t flc_time;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};


