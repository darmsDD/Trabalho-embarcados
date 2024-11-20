/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <rosidl_runtime_c/string.h>

#include <time.h>

#include <sensor_msgs/msg/joint_state.h>             // for the encoder msg
#include <geometry_msgs/msg/twist.h>                 // for the motors control

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern UART_HandleTypeDef hlpuart1;

/* Subscriber declaration */
rcl_subscription_t cmd_vel_sub;

/* Publisher declaration */
rcl_publisher_t joint_state_pub;

/* ROS timer declaration */
rcl_timer_t timer;

/* Messages declaration */
sensor_msgs__msg__JointState joint_state_msg;
geometry_msgs__msg__Twist cmd_vel_msg;

/* USER CODE END Variables */
/* Definitions for microROSTask */
osThreadId_t microROSTaskHandle;
const osThreadAttr_t microROSTask_attributes = {
  .name = "microROSTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 3000 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

void timer_callback(rcl_timer_t * timer, int64_t last_call_time);
void cmd_vel_callback(const void * msgin);

//extern int clock_gettime( int clock_id, struct timespec * tp );
extern void UTILS_NanosecondsToTimespec( int64_t llSource, struct timespec * const pxDestination );
/* USER CODE END FunctionPrototypes */

void microROSTaskFunction(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of microROSTask */
  microROSTaskHandle = osThreadNew(microROSTaskFunction, NULL, &microROSTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_microROSTaskFunction */
/**
  * @brief  Function implementing the microROSTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_microROSTaskFunction */
void microROSTaskFunction(void *argument)
{
  /* USER CODE BEGIN microROSTaskFunction */

  // micro-ROS configuration
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

  // micro-ROS app
  /*rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;
  rclc_executor_t executor;

  allocator = rcl_get_default_allocator();

  //create init_options
  rclc_support_init(&support, 0, NULL, &allocator);

  // create node
  rclc_node_init_default(&node, "acquisition_system", "", &support);
  // It is not possible to see the nodes and topics in the ros2 terminal neither in the rqt
  // This problem due ROS_DOMAIN_ID. To "solve" it run unset ROS_DOMAIN_ID.
  // Details are described here https://github.com/micro-ROS/micro_ros_arduino/issues/21*/



  /*rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;
  rclc_executor_t executor;
  rcl_init_options_t init_options;
  size_t domain_id;

  allocator = rcl_get_default_allocator();
  init_options = rcl_get_zero_initialized_init_options();
  rcl_init_options_init(&init_options, allocator);

  domain_id = 25;
  if(rcl_init_options_set_domain_id(&init_options, domain_id) != RCL_RET_OK)
  {
	  printf("Error on rcl_init_options_set_domain_id (line %d)\n", __LINE__);
  }

  // create init_options
  rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);

  // create node
  rclc_node_init_default(&node, "acquisition_system", "", &support);*/



  rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;
  rclc_executor_t executor;
  rcl_init_options_t init_options;

  allocator = rcl_get_default_allocator();
  init_options = rcl_get_zero_initialized_init_options();
  rcl_init_options_init(&init_options, allocator);

  // create init_options
  rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);

  // create node
  rcl_node_options_t node_ops = rcl_node_get_default_options();
  node_ops.domain_id = 25;
  rclc_node_init_with_options(&node, "acquisition_system", "", &support, &node_ops);


  //time sync
  if( rmw_uros_sync_session(1000) != RMW_RET_OK)
	  printf("Error on time sync (line %d)\n", __LINE__);

  //int64_t time_ns;
  //time_ns = rmw_uros_epoch_nanos();


  // create cmd_vel_sub
  cmd_vel_sub = rcl_get_zero_initialized_subscription();
  rclc_subscription_init_best_effort(
		  &cmd_vel_sub,
		  &node,
		  ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
		  "/cmd_vel");

  // create joint_state_pub
  rclc_publisher_init_default(
	&joint_state_pub,
	&node,
	ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
	"/joint_state");

  // joint_state message allocation. Described in https://micro.ros.org/docs/tutorials/advanced/handling_type_memory/
  joint_state_msg.header.frame_id.capacity = 20;
  joint_state_msg.header.frame_id.data = (char*) pvPortMalloc(joint_state_msg.header.frame_id.capacity  * sizeof(char));
  joint_state_msg.header.frame_id.size = strlen(joint_state_msg.header.frame_id.data);

  joint_state_msg.name.capacity = 2;
  joint_state_msg.name.data = (rosidl_runtime_c__String*) pvPortMalloc(joint_state_msg.name.capacity * sizeof(rosidl_runtime_c__String));
  joint_state_msg.name.size = 2;

	joint_state_msg.name.data[0].capacity = 20;
	joint_state_msg.name.data[0].data = (char*) pvPortMalloc(joint_state_msg.name.data[0].capacity * sizeof(char));
	strcpy(joint_state_msg.name.data[0].data, "Roda_L_Joint");
	joint_state_msg.name.data[0].size = strlen(joint_state_msg.name.data[0].data);
	joint_state_msg.name.data[1].capacity = 20;
	joint_state_msg.name.data[1].data = (char*) pvPortMalloc(joint_state_msg.name.data[1].capacity* sizeof(char));
	strcpy(joint_state_msg.name.data[1].data, "Roda_R123_Joint");
	joint_state_msg.name.data[1].size = strlen(joint_state_msg.name.data[1].data);
	joint_state_msg.name.size=2;

	joint_state_msg.position.capacity = 2;
	joint_state_msg.position.data = (double*) pvPortMalloc(joint_state_msg.position.capacity * sizeof(double));
	joint_state_msg.position.data[0] = 0;
	joint_state_msg.position.data[1] = 0;
	joint_state_msg.position.size = 2;

  joint_state_msg.velocity.capacity = 2;
  joint_state_msg.velocity.data = (double*) pvPortMalloc(joint_state_msg.velocity.capacity * sizeof(double));
  joint_state_msg.velocity.data[0] = 0;
  joint_state_msg.velocity.data[1] = 0;
  joint_state_msg.velocity.size = 2;

  joint_state_msg.effort.capacity = 2;
  joint_state_msg.effort.data = (double*) pvPortMalloc(joint_state_msg.effort.capacity * sizeof(double));
  joint_state_msg.effort.data[0]=-1;
  joint_state_msg.effort.data[1]=-1;
  joint_state_msg.effort.size = 2;

  // Create a timer
  rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(10), timer_callback);

  // Create executor
  rclc_executor_init(&executor, &support.context, 2, &allocator);
  rclc_executor_add_subscription(&executor, &cmd_vel_sub, &cmd_vel_msg,
		  &cmd_vel_callback, ON_NEW_DATA); // ON_NEW_DATA does not work properly
  rclc_executor_add_timer(&executor, &timer);

  // Run executor
  rclc_executor_spin(&executor);

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END microROSTaskFunction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
	if (timer != NULL) {
		// Blink the LED1 (yellow) for debugging
		//HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);

		// Fill the message timestamp
		struct timespec ts;
		int64_t time_ns;
		time_ns = rmw_uros_epoch_nanos();
		UTILS_NanosecondsToTimespec(time_ns, &ts);
		//clock_gettime(CLOCK_REALTIME, &ts);

		// Create the Header
		joint_state_msg.header.stamp.sec = ts.tv_sec;
		joint_state_msg.header.stamp.nanosec = ts.tv_nsec;

		//sprintf(joint_state_msg.header.frame_id.data, "%ld", seq_no);
		//joint_state_msg.header.frame_id.size = strlen(joint_state_msg.header.frame_id.data);

		joint_state_msg.position.data[0] = 1;
		joint_state_msg.position.data[1] = 2;
		joint_state_msg.velocity.data[0] = 3;
		joint_state_msg.velocity.data[1] = 4;
		joint_state_msg.effort.data[0] = 5;
		joint_state_msg.effort.data[1] = 6;

		// Publish the message
		rcl_ret_t ret = rcl_publish(&joint_state_pub, &joint_state_msg, NULL);
		if (ret != RCL_RET_OK)
		{
		  printf("Error publishing joint_state (line %d)\n", __LINE__);
		}
	}
}

void cmd_vel_callback(const void * msgin)
{
	double leftWheelVelocity, rightWheelVelocity;
	double linearX, linearY, linearZ, angularX, angularY, angularZ;
	const geometry_msgs__msg__Twist * cmd_vel_msg;

	if (msgin != NULL)
	{

		// Blink the LED2 (orange) for debugging
		HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);

		cmd_vel_msg = (const geometry_msgs__msg__Twist *)msgin;

		linearX = cmd_vel_msg->linear.x;
		linearY = cmd_vel_msg->linear.y;
		linearZ = cmd_vel_msg->linear.z;
		angularX = cmd_vel_msg->angular.x;
		angularY = cmd_vel_msg->angular.y;
		angularZ = cmd_vel_msg->angular.z;
	}
}
/* USER CODE END Application */

