/*
 * Copyright (C) 2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gz/msgs/actuators.pb.h>
#include <gz/msgs/laserscan.pb.h>
#include <gz/transport/Node.hh>
#include <unistd.h>


gz::transport::Node node;
std::string topic_pub = "/X3/gazebo/command/motor_speed";
gz::msgs::Actuators data;
auto pub = node.Advertise<gz::msgs::Actuators>(topic_pub);


void go_up(){
    data.clear_velocity();	
    pub.Publish(data);
}

void go_down(){
    // data[0].clear_velocity();
    // data[1].clear_velocity();
    // data[2].clear_velocity();
    // data[3].clear_velocity();	
    // pub.Publish(data[0]);
    // pub.Publish(data[1]);
    // pub.Publish(data[2]);
    // pub.Publish(data[3]);
    data.add();
    pub.Publish(data);
}


int main(int argc, char **argv)
{
    std::string topic = "/lidar";
    // Subscribe to a topic by registering a callback.
    bool up = true;
    while(1){
        if(up){
            go_down();
        }else if(!up){
            go_up();
        }
        sleep(5);
    }

    // Zzzzzz.
    gz::transport::waitForShutdown();

    return 0;
}