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
    lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t now = simTime();
        simtime_t creationTime = msg->getCreationTime();

        // Debugging creation time
        if (creationTime == SimTime(0) || creationTime < 0) {
               EV << "Invalid creation time for message: " << msg->getName() << endl;
               delete msg;
               return;
           }

        simtime_t lifetime = now - creationTime;
        if (lifetime == SimTime(0) || lifetime < 0) {
                EV << "Invalid lifetime: " << lifetime << endl;
                delete msg;
                return;
            }
    sumLifetimes += lifetime.dbl();  // Update total lifetime
    messageCount++;                  // Increment message count

    if (lifetime > maxLifetime) {
        maxLifetime = lifetime.dbl();  // Update max if current lifetime is greater
    }
    if (lifetime < minLifetime) {
        minLifetime = lifetime.dbl();  // Update min if current lifetime is smaller
    }

      delayTraces.push_back(lifetime.dbl());
      EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
      emit(lifetimeSignal, lifetime);
      delete msg;
}
void Sink::finish() {
    double meanLifetime = messageCount > 0 ? sumLifetimes / messageCount : 0;

    // Record scalar values for mean, max, and min lifetimes
    recordScalar("meanLifetime", meanLifetime);
    recordScalar("maxLifetime", maxLifetime);
    recordScalar("minLifetime", minLifetime);
    recordScalar("messageCount", messageCount);

    // Write traces to a file
    std::ofstream outFile("C:\\Users\\Edward\\Desktop\\Homework.txt"); // Opens a file in write mode
    outFile << "Max Lifetime: " << maxLifetime << "s\n";
         outFile << "Min Lifetime: " << minLifetime << "s\n";
         outFile << "Average Lifetime: " << (sumLifetimes / messageCount) << "s\n";
    if (outFile.is_open()) {
        outFile << "Lifetime traces for all messages:\n";
        for (double delay : delayTraces) {
            outFile << delay << "\n"; // Write each delay on a new line
        }
        outFile.close(); // Close the file after writing
    } else {
        EV << "Error: Could not open file for writing!" << endl;
    }

}

