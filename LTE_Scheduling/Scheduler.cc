#include "Scheduler.h"
#include "MyQ.h"
#include <iomanip>
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
    NrOfChannels = 10;//read from omnetpp.ini
    selfMsg = new cMessage("selfMsg");
    r.resize(NrUsers, 1.0);
    T.resize(NrUsers, 0.0);
    p.resize(NrUsers, 0.0); //  product of its radio link quality r[i] and the time elapsed since it was served last time T[i]
    for(int i=0; i<NrUsers;i++){
           q[i]=0;
           NrBlocks[i]=0;
    }
    userWeights[0] = 1;
    userWeights[1] = 8;
    userWeights[2] = 8;
    userWeights[3] = 2;
    userWeights[4] = 1;
    scheduleAt(simTime(), selfMsg);
}

void Scheduler::handleMessage(cMessage *msg)
{

        double totalDelayHP = 0;
        int countHP = 0;
        EV << "Nr users" << NrUsers << endl;
        if (msg == selfMsg){

        for (int idx=0;idx<NrUsers;idx++) {
            if(userWeights[idx]==8){
                totalDelayHP += simTime().dbl() - T[idx];
                countHP++;
                 EV << "sim[" << idx << "] = " << std::fixed << std::setprecision(6) << simTime().dbl() << " t[" << idx << "] = " << std::fixed << std::setprecision(6) << T[idx] << endl;

            }

        }
        EV << "totalDelayHP = " << totalDelayHP << endl;


        double avgDelayHP = (countHP > 0) ? (totalDelayHP / countHP) : 0;
        W_HP += computeWHPAdjustment(avgDelayHP);
        W_HP = std::max(0.5, std::min(W_HP, 8.0));  // Keep W_HP in range [0.5, 2.0]
        EV << "Updated W_HP = " << W_HP << endl;
        // Step 1: Get queue lengths and randomly generate channel quality
        for(int j=0; j<NrUsers; j++){
            q[j]= getParentModule()->getSubmodule("user",j)->getSubmodule("myqq")->par("qlp");
            r[j] = uniform(0.1, 1.0); // Random quality between 0.1 and 1.0
            //EV << "queue length q["<<j<<"]= " << q[j] <<endl;
        }

        std::vector<double> p(NrUsers);   // PF metric for each user

            // Step 2: Compute the PF metric for each user
        for (int i = 0; i < NrUsers; i++) {
            double timeSinceLastServed = simTime().dbl() - T[i];
            int weight = userWeights[i];
            if(userWeights[i]==8){
                weight=W_HP;
            }else{
                weight=1;
            }
            p[i] = r[i] * timeSinceLastServed *weight;
            EV << "Scheduler: Updated PT p[" << i << "] = " << std::fixed << std::setprecision(6) << p[i] << endl;
            EV << "r[" << i << "] = " << std::fixed << std::setprecision(6) << r[i] << " t[" << i << "] = " << std::fixed << std::setprecision(6) << T[i] << endl;
         }

        // Step 3: Sort users by their PF metric in descending order
            std::vector<int> indices(NrUsers);
            for (int i = 0; i < NrUsers; i++) {
                indices[i] = i;
            }
            std::sort(indices.begin(), indices.end(),
                      [&p](int a, int b) { return p[a] > p[b]; });

       // Step 4: Allocate blocks to users based on sorted order
            int remainingChannels = NrOfChannels;
            for (int idx : indices) {
                if (remainingChannels > 0 && q[idx] > 0) {
                    if(q[idx] > remainingChannels)
                       NrBlocks[idx] = remainingChannels;
                    else
                       NrBlocks[idx] = q[idx];
                    q[idx] -= NrBlocks[idx];
                    T[idx] = simTime().dbl(); // Update last served time
                    remainingChannels -= NrBlocks[idx];

                    // Create and send a command message
                    cMessage *cmd = new cMessage("cmd");
                    cmd->addPar("nrBlocks");
                    cmd->par("nrBlocks").setLongValue(NrBlocks[idx]);
                    send(cmd, "txScheduling", idx);

                    EV << "Allocated " << NrBlocks[idx] << " blocks to user " << idx << ", remaining channels: " << remainingChannels << endl;
                }
            }
        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);


    }

}
double Scheduler::computeWHPAdjustment(double avgDelay) {
    double low = lowDelay(avgDelay);
    double medium = mediumDelay(avgDelay);
    double high = highDelay(avgDelay);

    double increaseSmall = medium;   // If delay is medium, slightly increase W_HP
    double keepSame = low;           // If delay is low, do not change W_HP
    double increaseLarge = high;     // If delay is high, significantly increase W_HP

    // Defuzzification: Weighted sum method
    double adjustment = (increaseSmall * 0.1 + keepSame * 0.0 + increaseLarge * 0.3) /
                        (increaseSmall + keepSame + increaseLarge + 1e-6); // Avoid division by zero

    return adjustment;
}

//Returns 1 if delay is very small (below half the threshold), and gradually drops to 0 as it approaches the threshold.
double Scheduler::lowDelay(double delay) {
    double threshold = par("HPDelayThreshold").doubleValue();
    EV << "threshold=" << threshold <<endl;

    return (delay < 0.5 * threshold) ? 1.0 : std::max(0.0, (threshold - delay) / (0.5 * threshold));
}
//Active only between 0.5 * threshold and threshold. Linearly increases from 0 to 1 between those points.
double Scheduler::mediumDelay(double delay) {
    double threshold = par("HPDelayThreshold").doubleValue();
    if (delay < 0.5 * threshold || delay > threshold)
        return 0.0;
    return (delay - 0.5 * threshold) / (0.5 * threshold);
}

//Returns 0 for low delays, ramps up after 0.5 * threshold, and hits 1 at threshold.
double Scheduler::highDelay(double delay) {
    double threshold = par("HPDelayThreshold").doubleValue();
    return (delay >= threshold) ? 1.0 : std::max(0.0, (delay - 0.5 * threshold) / (0.5 * threshold));
}

