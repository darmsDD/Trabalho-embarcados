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

#include <gz/msgs/twist.pb.h>
#include <gz/msgs/laserscan.pb.h>
#include <gz/transport/Node.hh>



gz::transport::Node node;
std::string topic_pub = "/cmd_vel";
gz::msgs::Twist data;
auto pub = node.Advertise<gz::msgs::Twist>(topic_pub);

void cb(const gz::msgs::LaserScan &_msg)
{
  std::cout << "Entrei no callback";
  bool allMore = true;
  for (int i = 0; i < _msg.ranges_size(); i++)
  {
    if (_msg.ranges(i) < 1.0)
    {
      allMore = false;
      break;
    }
  }
  if (allMore) //if all bigger than one
  {
    data.mutable_linear()->set_x(0.5);
    data.mutable_angular()->set_z(0.0);
  }
  else
  {
    data.mutable_linear()->set_x(0.0);
    data.mutable_angular()->set_z(0.5);
  }
  pub.Publish(data);
}


int main(int argc, char **argv)
{
    std::string topic = "/imu";
    // Subscribe to a topic by registering a callback.
    if (!node.Subscribe(topic, cb))
    {
        std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
        return -1;
    } else{

    }

    // Zzzzzz.
    gz::transport::waitForShutdown();

    return 0;
}