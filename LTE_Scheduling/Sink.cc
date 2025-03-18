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
        meanHPDelay=0.0;
        meanMPDelay=0.0;
}

void Sink::handleMessage(cMessage *msg)
{
    int NrUsers = par("gateSize").intValue();
        simtime_t lifetime = simTime() - msg->getCreationTime();
        double alpha = 0.7;

        for(int i=0;i < NrUsers;i++){
            if (msg->arrivedOn("rxPackets",i)) {
                EV << "Message arrived on rxPackets[" << i << "]" << endl;

                if(i >= round(NrUsers*0.7)){
                    double ms = lifetime.dbl() * 1000;
                    if(meanHPDelay == 0.0){
                        meanHPDelay = ms;
                    }
                    else{
                        meanHPDelay = alpha * meanHPDelay + (alpha-1) * ms;
                    }

                    par("meanHPDelay") = meanHPDelay;
                    EV << "Mean HP delay: " << meanHPDelay << endl;

                }
                else if (i >= round(NrUsers * 0.3)){
                    double ms = lifetime.dbl() * 1000;

                    if(meanMPDelay == 0.0){
                        meanMPDelay = ms;
                    }
                    else{
                        meanMPDelay = alpha * meanMPDelay + (alpha-1) * ms;
                    }

                    par("meanMPDelay") = meanMPDelay;
                    EV << "Mean MP delay: " << meanMPDelay << endl;
                }

                emit(lifetimeSignals[i], lifetime);
                break;
            }
        }

          EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
          delete msg;
}
