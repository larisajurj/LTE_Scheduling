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
    ASSERT(msg == sendMessageEvent);
    int i =  getParentModule()->getIndex();
    cMessage *job = new cMessage("job");
    EV<<"Sending new packet for user "<<i<<endl;
    send(job, "txPackets");

    double sendingTime;
    sendingTime = par("sendIaTime").doubleValue();
    scheduleAt(simTime()+ exponential(sendingTime), sendMessageEvent);

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
