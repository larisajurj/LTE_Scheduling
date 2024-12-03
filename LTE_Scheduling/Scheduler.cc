//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

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
    NrOfChannels = 10;//read from omnetpp.ini
    selfMsg = new cMessage("selfMsg");
    r.resize(NrUsers, 1.0);
    T.resize(NrUsers, 1.0);
    p.resize(NrUsers, 0.0); //  product of its radio link quality r[i] and the time elapsed since it was served last time T[i]
    for(int i=0; i<10;i++){
           q[i]=0;
           NrBlocks[i]=0;
    }
    scheduleAt(simTime(), selfMsg);
}

void Scheduler::handleMessage(cMessage *msg)
{
        int q[NrUsers];
        int NrBlocks[NrUsers];

        // Step 1: Get queue lengths and randomly generate channel quality
        for(int j=0; j<NrUsers; j++){
            q[j]= getParentModule()->getSubmodule("user",j)->getSubmodule("myqq")->par("qlp");
            r[j] = uniform(0.1, 1.0); // Random quality between 0.1 and 1.0
            EV << "queue length q["<<j<<"]= " << q[j] <<endl;
        }

        if (msg == selfMsg){
            std::vector<double> r(NrUsers);   // Channel quality for each user
            std::vector<double> p(NrUsers);   // PF metric for each user

        // Step 2: Compute the PF metric for each user
            for (int i = 0; i < NrUsers; i++) {
                double timeSinceLastServed = simTime().dbl() - T[i];
                p[i] = r[i] * timeSinceLastServed;
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
                    NrBlocks[idx] = std::min(2, remainingChannels); // Allocate up to 2 blocks
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
