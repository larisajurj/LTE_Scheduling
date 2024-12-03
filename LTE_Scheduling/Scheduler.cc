#include "Scheduler.h"
#include <numeric>
#include <algorithm>

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
    totalQueueSignal = registerSignal("totalQueueSignal");
    rbAllocationSignal = registerSignal("rbAllocationSignal");
    r.resize(NrUsers, 1.0);
    T.resize(NrUsers, 1.0);
    p.resize(NrUsers, 0.0);
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
double Scheduler::sumNrBlocks() {
    double sum = 0.0;
    for (int i = 0; i < 10; i++) { // Replace 10 with the actual size of the NrBlocks array
        sum += NrBlocks[i];
    }
    return sum;
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
                T[i] += par("schedulingPeriod").doubleValue();
            }
            updateRadioQuality();
            computePFMetric();
            emit(totalQueueSignal, std::accumulate(q, q + NrUsers, 0.0));
            emit(rbAllocationSignal, sumNrBlocks());
            scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
        }
    if (selfMsg != nullptr) {
                  cancelEvent(selfMsg);  // Cancel any previously scheduled event
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
void Scheduler::updateRadioQuality() {
    for (int i = 0; i < NrUsers; i++) {
        r[i] = uniform(0.1, 1.0);
        EV << "Updated r[" << i << "] = " << r[i] << endl;
    }
}
void Scheduler::computePFMetric() {
    for (int i = 0; i < NrUsers; i++) {
        p[i] = r[i] * T[i];  // PF metric
        EV << "Computed p[" << i << "] = " << p[i] << endl;
    }

    // Sort users by decreasing p[i]
    std::vector<int> userOrder(NrUsers);
    std::iota(userOrder.begin(), userOrder.end(), 0);  // Initialize user indices

    std::sort(userOrder.begin(), userOrder.end(), [&](int a, int b) {
        return p[a] > p[b];
    });

    // Allocate RBs in sorted order
    int remainingRBs = NrOfChannels;
    for (int user : userOrder) {
        int allocatedRBs = std::min(rbPerUser, remainingRBs);
        NrBlocks[user] = allocatedRBs;
        remainingRBs -= allocatedRBs;

        // Reset timer for the user
        if (allocatedRBs > 0)
            T[user] = 0.0;
    }
}

