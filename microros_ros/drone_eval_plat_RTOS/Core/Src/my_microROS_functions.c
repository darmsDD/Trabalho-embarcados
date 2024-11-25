/*
 * my_base_functions.c
 *
 *  Created on: Nov 24, 2024
 *      Author: ivan
 */


#include "my_microROS_functions.h"

/* Subscriber declaration */
rcl_subscription_t imu_sub;

/* Publisher declaration */
rcl_publisher_t velocity_pub;

/* ROS timer declaration */
rcl_timer_t timer;

/* Messages declaration */
actuator_msgs__msg__Actuators velocity_msg;
sensor_msgs__msg__Imu imu_msg;

/* Microros global variables*/
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;


const int iNumberOfTries = 1;
rcl_ret_t i32PubMessageState = RCL_RET_OK;

/* *********************************************************************
	Method name:

 	Method description:

    Input params:       variable_name


	Output params:       n/a

	Extra:

*************************************************************************/






/* *********************************************************************
	Method name: vSetActuatorMsg
 	Method description:  This method is responsable for
                      	 setting the velocity_msg. Which
                        will be sent to the HOST (gazebo)

    Input params:       *fpVelocity
                        A new velocity variable for the drone.
                        float pointer of size 4
                        [0] and [1] are the motors in this position /
                        left motor at the bottom and right motor is at
                        the top.[2] and [3] are the motors in this
                        position \, left motor at the top and right
                        motor is at the bottom.

	Output params:       n/a

	Extra: this msg is composed by, but we are only using velocity
	 * header : { stamp: {sec:0,nanosec:0}, frame_id:''}
	 * position: []
	 * velocity: []
	 * normalized: []

*************************************************************************/

void vSetActuatorMsg(float *fpVelocity){
	velocity_msg.velocity.data[0] = fpVelocity[0];
	velocity_msg.velocity.data[1] = fpVelocity[1];
	velocity_msg.velocity.data[2] = fpVelocity[2];
	velocity_msg.velocity.data[3] = fpVelocity[3];
	velocity_msg.velocity.data[4] = imu_msg.linear_acceleration.z;
}

/* *********************************************************************
	Method name: vFirstSetActuatorMsg

 	Method description: This method is used to allocate memory and set
 						initial values to the velocity_msg variable.
 						The variable velocity_msg will be used to send
 						velocities to the drone in gazebo.

    Input params:       *fpVelocity
                        A new velocity variable for the drone.
                        float pointer of size 4
                        [0] and [1] are the motors in this position /
                        left motor at the bottom and right motor is at
                        the top.[2] and [3] are the motors in this
                        position \, left motor at the top and right
                        motor is at the bottom.


	Output params:       n/a

	Extra:				This method is necessary, because if we keep allocating
   	   	   	   	   	   	memory with every message sent, at some point we will
   	   	   	   	   	   	run out of memory.

*************************************************************************/

void vFirstSetActuatorMsg(float *fpVelocity){
	velocity_msg.header.frame_id.capacity = 20;
	velocity_msg.header.frame_id.data = (char*) pvPortMalloc(velocity_msg.header.frame_id.capacity  * sizeof(char));
	velocity_msg.header.frame_id.size = strlen(velocity_msg.header.frame_id.data);

	velocity_msg.velocity.capacity = 5;
	velocity_msg.velocity.data = (double*) pvPortMalloc(velocity_msg.velocity.capacity * sizeof(double));
	velocity_msg.velocity.data[0] = fpVelocity[0];
	velocity_msg.velocity.data[1] = fpVelocity[1];
	velocity_msg.velocity.data[2] = fpVelocity[2];
	velocity_msg.velocity.data[3] = fpVelocity[3];
	velocity_msg.velocity.data[4] = imu_msg.linear_acceleration.z;
	velocity_msg.velocity.size = 5;
}

/* *********************************************************************
	Method name:	 	vSendActuatorMsg

 	Method description:	Sends the velocity to gazebo using the microROS
 						wrapper. If the message can not be sent, the
 						variable i32PubMessageState is used to restart
 						the microROS wrapper and fix a possible error.

    Input params:       n/a


	Output params:       n/a

	Extra:

*************************************************************************/
void vSendActuatorMsg(){
	i32PubMessageState = rcl_publish(&velocity_pub, &velocity_msg, NULL);
}

/* *********************************************************************
	Method name:		vImuCallback

 	Method description: This is method is called when a message from
 						gazebo arrives. It sets the thread flag from the
 						task readFromHost to 1, allowing the new value from
 						the simulated imu (imu_msg) to be used.

    Input params:       const void * msgin


	Output params:       n/a

	Extra:

*************************************************************************/
//void vImuCallback(const void * msgin)
//{
//	//const sensor_msgs__msg__Imu * minha_msg;
//	if (msgin != NULL)
//	{
//		// Indica que houve leitura da IMU para a tarefa escrever setpoint
//		osThreadFlagsSet(readFromHostHandle, 0x01);
//		//float a_velocity[] = {5,10,100,500};
//		//vSetActuatorMsg(a_velocity);
//		HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);
//	}
//
//}
void vImuCallback(const void * msgin)
{
	//const sensor_msgs__msg__Imu * minha_msg;
	if (msgin != NULL)
	{
		// Indica que houve leitura da IMU para a tarefa escrever setpoint
		osEventFlagsSet(hostImuEventHandle,0x01);
		//uint32_t valor_flag = osThreadFlagsSet(readFromHostHandle, 0x01);
		//float a_velocity[] = {5,10,100,500};
		//vSetActuatorMsg(a_velocity);
		//HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);
	}

}

/* *********************************************************************
	Method name:		vMyMicroros

 	Method description: This method is responsible for creating the
 						microROS wrapper and restarting it in case of
 						errors. Steps:
 						1- Initial microROS configuration.
 						2- Creates Node.
 						3- Synchronizes the session timer.
 						4- Creates a publisher.
 						5- Creates a subscriber.
 						6- Creates an executor.
						7- If an error occurs, cleans up everything and exit.
    Input params:       n/a


	Output params:       n/a

	Extra:

*************************************************************************/
void vMyMicroros(){
	if(i32MicrorosConfiguration()==RCL_RET_OK && i32CreateNode()==RCL_RET_OK && rmw_uros_sync_session(1000) == RMW_RET_OK &&
			i32CreatePublisher()==RCL_RET_OK && i32CreateSubscriber()==RCL_RET_OK && i32CreateExecutor()==RCL_RET_OK){

		float a_velocity[] = {0,0,0,0};
		vFirstSetActuatorMsg(a_velocity);
		/*
		 * The function rclc_executor_spin runs forever without coming back. To allow checkig if an
		 * error ocurred, we are using the spin-method rclc_executor_spin_some, which spins only once
		 * and returns. The wait timeout for checking for new messages at the DDS-queue or waiting timers to get ready
		 * is configured to be one second.
		 * https://micro.ros.org/docs/tutorials/programming_rcl_rclc/executor/
		 */

		// Run executor
		rcl_ret_t ret = rclc_executor_spin_some(&executor,1000*(1000*1000));
		//rclc_executor_spin(&executor);
		while(RCL_RET_OK==ret){

			if(i32PubMessageState!=RCL_RET_OK){
				break;
			}
			ret = rclc_executor_spin_some(&executor,1000*(1000*1000));
		}

	}
	rcl_ret_t rc;
	rc = rclc_executor_fini(&executor);
	rc += rcl_publisher_fini(&velocity_pub, &node);
	rc += rcl_timer_fini(&timer);
	rc += rcl_subscription_fini(&imu_sub, &node);
	rc += rcl_node_fini(&node);
	rc += rclc_support_fini(&support);
	i32PubMessageState=RCL_RET_OK;
	vPortFree((void *)velocity_msg.header.frame_id.data); // Necessary to avoid running out of memory.
	vPortFree((void *)velocity_msg.velocity.data);	// Necessary to avoid running out of memory.
	vFastBlinkOnError();
	return;
}

/* *********************************************************************
	Method name:		i32MicrorosConfiguration

 	Method description:	This method configures the microROS wrapper. For
 	 	 	 	 	 	most functions, the system tries it "iNumberOfTries"
 	 	 	 	 	 	before exiting.

    Input params:       n/a


	Output params:      rcl_ret_t ret
   	   	   	   	   	   	If ret is RCL_RET_OK, this method was successful.

	Extra:

*************************************************************************/
rcl_ret_t i32MicrorosConfiguration(){
	rmw_uros_set_custom_transport(
	true,
	(void *) &hlpuart1,
	cubemx_transport_open,
	cubemx_transport_close,
	cubemx_transport_write,
	cubemx_transport_read);

	rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
	freeRTOS_allocator.allocate = microros_allocate;
	freeRTOS_allocator.deallocate = microros_deallocate;
	freeRTOS_allocator.reallocate = microros_reallocate;
	freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

	if (!rcutils_set_default_allocator(&freeRTOS_allocator)) {
	  printf("Error on default allocators (line %d)\n", __LINE__);
	}

	allocator = rcl_get_default_allocator();
	// Initialize and modify options (Set DOMAIN ID to 25)
	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();

	int i=0;
	rcl_ret_t ret;
	ret = rcl_init_options_init(&init_options, allocator);
	while (ret != RCL_RET_OK && i<iNumberOfTries){
		ret = rcl_init_options_init(&init_options, allocator);
		i++;
	}
	if(ret != RCL_RET_OK){return ret;}


	i=0;
	ret = rcl_init_options_set_domain_id(&init_options, 25);
	while (ret != RCL_RET_OK && i<iNumberOfTries){
		ret = rcl_init_options_set_domain_id(&init_options, 25);
		i++;
	}
	if(ret != RCL_RET_OK){return ret;}

	// Initialize rclc support object with custom options
	i=0;
	ret = rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
	while (ret != RCL_RET_OK && i<iNumberOfTries){
		ret = rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
		i++;
	};


	return ret;
}

/* *********************************************************************
	Method name:		i32CreateNode

 	Method description: This method creates a ROS node.

    Input params:       n/a


	Output params:       rcl_ret_t rc2
   	   	   	   	   	    If rc2 is RCL_RET_OK, this method was successful.

	Extra:

*************************************************************************/
rcl_ret_t i32CreateNode(){
	// Create node object

	const char * node_name = "test_node";
	// Node namespace (Can remain empty "")
	const char * namespace = "";
	int i=0;
	// Init node with configured support object
	rcl_ret_t rc2 = rclc_node_init_default(&node, node_name, namespace, &support);
	while(rc2 != RCL_RET_OK && i<iNumberOfTries){
		rc2 = rclc_node_init_default(&node, node_name, namespace, &support);
		i++;
	}
	return rc2;
}

/* *********************************************************************
	Method name:		i32CreatePublisher

 	Method description: This method creates a ROS publisher.

    Input params:       n/a


	Output params:      rcl_ret_t ret
   	   	   	   	   	    If ret is RCL_RET_OK, this method was successful.

	Extra:				/X3/gazebo/command/motor_speed is the topic
						generated by gazebo for the drone motors. We use
						this topic to write the speed of the motors.
*************************************************************************/
rcl_ret_t i32CreatePublisher(){
	rcl_ret_t ret = rclc_publisher_init_default(
			&velocity_pub,
			&node,
			ROSIDL_GET_MSG_TYPE_SUPPORT(actuator_msgs,msg,Actuators),
			"/X3/gazebo/command/motor_speed"
		);
	int i=0;
	while (ret != RCL_RET_OK && i<iNumberOfTries){
		ret = rclc_publisher_init_default(
					&velocity_pub,
					&node,
					ROSIDL_GET_MSG_TYPE_SUPPORT(actuator_msgs,msg,Actuators),
					"/X3/gazebo/command/motor_speed"
			   );
		i++;
	};

	 return ret;
}

/* *********************************************************************
	Method name:		i32CreateSubscriber

 	Method description: This method creates a ROS subscriber.

    Input params:       n/a


	Output params:      rcl_ret_t rc_imu
   	   	   	   	   	    If rc_imu is RCL_RET_OK, this method was successful.

	Extra:				/drone/imu is the topic generated by gazebo for
						the imu . We use this topic to read information
						about the drone imu.
*************************************************************************/
rcl_ret_t i32CreateSubscriber(){
	 const char * imu_topic_name = "/drone/imu";
	// Get message type support
	const rosidl_message_type_support_t * imu_type_support =
	  ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs,msg,Imu);
	// Initialize a reliable subscriber
	rcl_ret_t rc_imu = rclc_subscription_init_default(
			&imu_sub, &node,
			imu_type_support, imu_topic_name
		);
	int i=0;
	while(RCL_RET_OK != rc_imu && i<iNumberOfTries){
		rc_imu = rclc_subscription_init_default(
			&imu_sub, &node,
			imu_type_support, imu_topic_name
		);
		i++;

	}
	return rc_imu;
}

/* *********************************************************************
	Method name:		i32CreateExecutor

 	Method description: This method creates rcl executor. It adds a
 						subscriber and a timer that publishes messages.
 						The executor is run by the function
 						rclc_executor_spin_some();

    Input params:       n/a


	Output params:      rcl_ret_t ret
   	   	   	   	   	    If ret is RCL_RET_OK, this method was successful.

	Extra:
*************************************************************************/
rcl_ret_t i32CreateExecutor(){
	 // Create a timer
	const unsigned int timer_timeout = 1000;
	rcl_ret_t ret;
	ret = rclc_timer_init_default2(&timer, &support, RCL_MS_TO_NS(timer_timeout), vSendActuatorMsg,true);
	if(ret != RCL_RET_OK){ return ret;}

	// Create executor
	ret = rclc_executor_init(&executor, &support.context, 2, &allocator);
	if(ret != RCL_RET_OK){ return ret;}

	ret = rclc_executor_add_subscription(&executor, &imu_sub, &imu_msg,
			  &vImuCallback, ON_NEW_DATA); // ON_NEW_DATA does not work properly
	if(ret != RCL_RET_OK){ return ret;}


	ret = rclc_executor_add_timer(&executor, &timer);

	return ret;
}

/* *********************************************************************
	Method name:		vFastBlinkOnError

 	Method description: This method is called to perform a visual indication
 						that an error has ocurred.
 						The LD2_Pin blinks at a 100ms rate for 5 seconds

    Input params:       n/a


	Output params:      n/a

	Extra:

*************************************************************************/
void vFastBlinkOnError(){
	for(int i=0;i<50;i++){
		HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);
		osDelay(100);
	}
}


xSetpoint xConvertQuaternionToAngle(geometry_msgs__msg__Quaternion xImuQuaternionAngles){
	xSetpoint xAngularPositions;
//	xHostData.fYaw = xImuQuaternionAngles.x;
//	xHostData.fRoll = xImuQuaternionAngles.z;
//	xHostData.fPitch = xImuQuaternionAngles.y;

//	var Θ = Math.Acos(W)*2;
	return xAngularPositions;
}

//typedef struct {
//  float fYaw;
//  float fRoll;
//  float fPitch;
//  int iDataFromJoystick;
//} xSetpoint;


