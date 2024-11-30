#include "Scheduler.h"

Define_Module(Scheduler);

Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}

void Scheduler::initialize()
{

    NrUsers = par("gateSize").intValue();
    NrOfChannels = par("totalRBs").intValue();
    rbPerUser = par("rbPerUser").intValue();
    if (rbPerUser * NrUsers > NrOfChannels) {
        EV << "Warning: Insufficient RBs to satisfy allocation!" << endl;
    }

    // Initialize queues and scheduling
    selfMsg = new cMessage("selfMsg");
    for (int i = 0; i < NrUsers; i++) {
        q[i] = 0;
    }


    scheduleAt(simTime(), selfMsg);
}

void Scheduler::handleMessage(cMessage *msg)
{

    if (msg != selfMsg) {
        for (int i = 0; i < NrUsers; i++) {
            if (msg->arrivedOn("rxInfo", i)) {
                q[i] = msg->par("ql_info");
                EV << "Update: q[" << i << "] = " << q[i] << endl;
                delete msg;
                return;
            }
        }
    }


    if (msg == selfMsg) {
        for (int i = 0; i < NrUsers; i++) {
            NrBlocks[i] = rbPerUser;
        }


        for (int i = 0; i < NrUsers; i++) {
            if (NrBlocks[i] > 0) {
                cMessage *cmd = new cMessage("cmd");
                cmd->addPar("nrBlocks");
                cmd->par("nrBlocks").setLongValue(NrBlocks[i]);
                send(cmd, "txScheduling", i);
                EV << "Allocated " << NrBlocks[i] << " RBs to User " << i << endl;
            }
        }

        scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
    }
}
