#include <gz/transport/Node.hh>
#include <gz/msgs/actuators.pb.h>
#include <gz/msgs/imu.pb.h>

#include <thread>
#include <chrono>

int a = 0;
gz::transport::Node node;
// Advertise on the motor speed topic
auto pub = node.Advertise<gz::msgs::Actuators>("/X3/gazebo/command/motor_speed");
#define NUMBER_OF_MOTORS 4
double d_motor_standard_velocity = 300.0;
void wait(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


// Helper function to publish motor speeds
void publishMotorSpeeds(double *pd_motor_speed, int i_number_of_motors) {
    gz::msgs::Actuators msg;
    for(int i=0;i<i_number_of_motors;i++){
        msg.add_velocity(pd_motor_speed[i]);
        //cout << pd_motor_speed[i] << "\n";
    }
    pub.Publish(msg);
}
void cb(const gz::msgs::IMU &_msg)
{
  a = 37;
}
int main() {
    // Initialize the Gazebo transport node
   

    // Wait for a subscriber to connect
    //pub.WaitForConnection();
    std::string topic = "drone/imu";
    // Subscribe to a topic by registering a callback.
    if (!node.Subscribe(topic, cb))
    {
        std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
        return -1;
    }


    while(1){

        
        double d_motor_speed[4] = {d_motor_standard_velocity,d_motor_standard_velocity/2.0,d_motor_standard_velocity/4.0,d_motor_standard_velocity/8.0};  // Speed for forward movement (adjust as needed)
        double d_motors_off[4] = {0.0,0.0,0.0,0.0};
       
        int i_motor_on_duration = 5000;     // Duration for each side (in milliseconds)
        int i_motor_off_duration = 1000;     // Duration for each turn (in milliseconds)

        
        publishMotorSpeeds(d_motor_speed,NUMBER_OF_MOTORS);
        wait(i_motor_on_duration);

        publishMotorSpeeds(d_motors_off,NUMBER_OF_MOTORS);
        wait(i_motor_off_duration);
        std::cout<<a << std::endl;

    }

    gz::transport::waitForShutdown();
    return 0;
}
