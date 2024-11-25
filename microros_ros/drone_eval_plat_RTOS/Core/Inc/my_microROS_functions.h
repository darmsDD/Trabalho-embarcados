/*
 * my_base_functions.h
 *
 *  Created on: Nov 23, 2024
 *      Author: ivan
 */

#ifndef INC_MY_MICROROS_FUNCTIONS_H_
#define INC_MY_MICROROS_FUNCTIONS_H_


#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <rosidl_runtime_c/string.h>

#include <time.h>

#include <sensor_msgs/msg/imu.h>             // for the encoder msg
#include <geometry_msgs/msg/twist.h>                 // for the motors control
#include <std_msgs/msg/int32.h>
#include <actuator_msgs/msg/actuators.h>

#include "main.h"
#include "cmsis_os.h"

//extern osThreadId_t readFromHostHandle;

extern osEventFlagsId_t hostImuEventHandle;
extern UART_HandleTypeDef hlpuart1;
/* Subscriber declaration */
extern rcl_subscription_t imu_sub;

/* Publisher declaration */
extern rcl_publisher_t velocity_pub;

/* ROS timer declaration */
extern rcl_timer_t timer;

/* Messages declaration */
extern actuator_msgs__msg__Actuators velocity_msg;
extern sensor_msgs__msg__Imu imu_msg;

/* Microros global variables*/
extern rcl_allocator_t allocator;
extern rclc_support_t support;
extern rcl_node_t node;
extern rclc_executor_t executor;

/* Extra variables */
extern const int iNumberOfTries;
extern rcl_ret_t i32PubMessageState;

// Minhas funções.

void vSetActuatorMsg(float *);
void vFirstSetActuatorMsg(float *);
void vSendActuatorMsg();
void vImuCallback(const void * msgin);
rcl_ret_t i32MicrorosConfiguration();
rcl_ret_t i32CreateNode();
rcl_ret_t i32CreatePublisher();
rcl_ret_t i32CreateSubscriber();
rcl_ret_t i32CreateExecutor();
void vFastBlinkOnError();
void vMyMicroros();


// Funções nativas.
bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);


#endif /* INC_MY_MICROROS_FUNCTIONS_H_ */
