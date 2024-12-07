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

#ifndef __SCHED_TST_SINK_H_
#define __SCHED_TST_SINK_H_

#include <omnetpp.h>
#include <vector>
#include <map>
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Sink : public cSimpleModule
{
private:
    simsignal_t lifetimeSignal;
    double sumLifetimes = 0;
    double maxLifetime = 0;
    double minLifetime = DBL_MAX;
    int messageCount = 0;
    std::vector<double> delayTraces;
    std::map<int, std::vector<double>> userDelayTraces;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual void finish();
public:
    Sink();


};
Sink::Sink() {
    for (int i = 0; i < 3; ++i) { // Adjust 3 to the number of users
        userDelayTraces[i] = std::vector<double>();
    }
}

#endif
