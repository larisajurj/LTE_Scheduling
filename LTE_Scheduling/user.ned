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
import MyQ;
import Source;

module User
{
    parameters:
    	//@display("i=block/user");

   // @display("bgb=90,160");

    gates:
        output txPackets;
        output txInfo;
        input rxScheduling;

    submodules:
        generator: Source;

        myqq: MyQ;

    connections:
        rxScheduling --> myqq.rxScheduling;
        generator.txPackets --> myqq.rxPackets;
        myqq.txPackets --> txPackets;
        myqq.txInfo --> txInfo;

}
