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

#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

void MyQ::handleMessage(cMessage *msg)

{

    int ql; //=9;

    if (msg->arrivedOn("rxPackets")){
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        //read parameters from msg
        //int nrOfRadioBlocks = (int)msg->par("nrOfBlocks");
        delete msg;
        //empty the queue !
        while(!queue.isEmpty()){ //for(int j = 0;j < nrOfRadioBlocks;j++ )
          msg = (cMessage *)queue.pop();
          send(msg, "txPackets");
        }
    }
    ql = queue.getLength();


    cMessage *qInfo = new cMessage("qInfo");
    qInfo->addPar("ql_info");
    qInfo->par("ql_info").setLongValue(ql);//ql instead of 2

    //only for test

    int i =  getParentModule()->getIndex();

    long ql_info_tst = qInfo->par("ql_info");
    EV << "ql[" << i<<"] = " << ql << " ql_info_tst = " << ql_info_tst << endl;
    send(qInfo, "txInfo");

    /* Communication between modules using parameters
    ql = queue.getLength();
    cPar&  qlpar = par("qlp");
    qlpar.setIntValue(ql);
    //next 2 statements only for debug
    int qt = par("qlp").intValue();
    EV << "qt= "<<qt <<endl;
    */

}
