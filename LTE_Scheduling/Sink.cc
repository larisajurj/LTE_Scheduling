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

#include "Sink.h"
#include <fstream>

Define_Module(Sink);

void Sink::initialize()
{
    int NrUsers = par("gateSize").intValue();
        for (int i = 0; i < NrUsers; i++) {
                std::string signalName = "lifetime_user" + std::to_string(i);
                lifetimeSignals.push_back(registerSignal(signalName.c_str()));
                EV << "Registered signal: " << signalName << endl;
            }
}

void Sink::handleMessage(cMessage *msg)
{
    int NrUsers = par("gateSize").intValue();
    simtime_t lifetime = simTime() - msg->getCreationTime();
    for(int i=0;i < NrUsers;i++){
        if (msg->arrivedOn("rxPackets",i)) {
            EV << "Message arrived on rxPackets[" << i << "]" << endl;
    // Emit the lifetime signal for the corresponding user
               emit(lifetimeSignals[i], lifetime);
               break; // Exit the loop after identifying the gate
           }
    }
    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    delete msg;
}
