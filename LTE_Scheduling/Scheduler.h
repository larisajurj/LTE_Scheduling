#ifndef __SCHED_TST_SCHEDULER_H_
#define __SCHED_TST_SCHEDULER_H_

#include <omnetpp.h>
#include <algorithm>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Scheduler : public cSimpleModule
{
public:
    Scheduler();
    ~Scheduler();
private:
    double W_HP = 8.0;  // Initial weight for HP users

        double computeWHPAdjustment(double avgDelay);
        double lowDelay(double delay);
        double mediumDelay(double delay);
        double highDelay(double delay);
    cMessage *selfMsg;
    int NrUsers;
    int NrOfChannels;
    int userWeights[10];
    int q[10];// queues' lengths. NrUsers schould be <= 10 !!!
    int NrBlocks[10];
    std::vector<double> r;
    std::vector<double> p;
    std::vector<double> T; // Time since last served for each user
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
