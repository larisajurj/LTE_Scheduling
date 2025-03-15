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

#include "Source.h"

Define_Module(Source);

Source::Source()
{
    sendMessageEvent = nullptr;
}

Source::~Source()
{
    cancelAndDelete(sendMessageEvent);
}

void Source::initialize()
{
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime(), sendMessageEvent);
}

void Source::handleMessage(cMessage *msg)


{
    int nrUsers = par("usersCount");
     int nrPackets = par("packetsPerUser");
     int PacketLength = 1;
     double schedulingCycle = 1.0;
     double netwload = par("networkLoad");
     int nrChannels = par("channels");
     for(int i=0;i<nrPackets;i++){
           ASSERT(msg == sendMessageEvent);
           cMessage *job = new cMessage("job");
           send(job, "txPackets");
       }
     if(simTime() >= par("MAX_Sim").doubleValue()){
               endSimulation();
           }
    double sendingTime;

     sendingTime = (nrUsers * nrPackets * PacketLength * schedulingCycle) / (netwload * nrChannels);

        // Output the calculated sending time for debugging
        EV << "Calculated Sending Time: " << sendingTime << " ms" << endl;

        scheduleAt(simTime() + exponential(sendingTime), sendMessageEvent);

}


/*
void Source::initialize()
{
    // TODO - Generated method body
}

void Source::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
*/
