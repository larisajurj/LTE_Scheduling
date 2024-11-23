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
import User;
import Sink;
import Scheduler;

network RadioSch2
{
parameters:
int usersCount; 
//= default(3);

submodules:
scheduler: Scheduler {
    gateSize = usersCount;
//@display("p=120,40");
}
sink: Sink {
gateSize = usersCount;
//@display("p=120,200");
}
user[usersCount]: User {
//@display("p=40,120");
}
connections: 
for i = 0..usersCount - 1 {
scheduler.txScheduling[i] --> user[i].rxScheduling;
user[i].txPackets --> sink.rxPackets[i];
user[i].txInfo --> scheduler.rxInfo[i];

}
}