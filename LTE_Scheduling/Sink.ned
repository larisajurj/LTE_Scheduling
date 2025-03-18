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

//
// TODO auto-generated module
//
simple Sink
{
 parameters:
        int gateSize;
        double meanHPDelay = default (1300.00) @mutable;
        double meanMPDelay = default (1700.00) @mutable;
        @display("i=block/sink");
        @signal[lifetime_user0](type="simtime_t");
        @signal[lifetime_user1](type="simtime_t");
        @signal[lifetime_user2](type="simtime_t");
        @signal[lifetime_user3](type="simtime_t");
        @signal[lifetime_user4](type="simtime_t");
        
        @statistic[lifetime_user0](title="Economy User 1 Packet Lifetime"; unit=s; record=vector,mean,max; interpolationmode=linear);
        @statistic[lifetime_user1](title="Premium User 1 Packet Lifetime"; unit=s; record=vector,mean,max; interpolationmode=linear);
        @statistic[lifetime_user2](title="Premium User 2 Packet Lifetime"; unit=s; record=vector,mean,max; interpolationmode=linear);
        @statistic[lifetime_user3](title="Standard User Packet Lifetime"; unit=s; record=vector,mean,max; interpolationmode=linear);
        @statistic[lifetime_user4](title="Economy User 2 Packet Lifetime"; unit=s; record=vector,mean,max; interpolationmode=linear);
    gates:
        input rxPackets[gateSize];

}
