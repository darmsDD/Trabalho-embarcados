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
#include <std_msgs/msg/int32.h>
#include <actuator_msgs/msg/actuators.h>
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
//std_msgs__msg__Int32 joint_state_msg;
actuator_msgs__msg__Actuators joint_state_msg;
//sensor_msgs__msg__JointState joint_state_msg;
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

//extern int clock_gettime( int clock_id, struct timespec * tp );
extern void UTILS_NanosecondsToTimespec( int64_t llSource, struct timespec * const pxDestination );
void setActuatorMsg(float *);
void sendActuatorMsg();
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

	rcl_allocator_t allocator = rcl_get_default_allocator();
	// Initialize and modify options (Set DOMAIN ID to 10)
	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
	rcl_init_options_init(&init_options, allocator);
	rcl_init_options_set_domain_id(&init_options, 25);
	// Initialize rclc support object with custom options
	rclc_support_t support;
	rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
	// Create node object
	rcl_node_t node;
	const char * node_name = "test_node";
	// Node namespace (Can remain empty "")
	const char * namespace = "test_namespace";
	// Init node with configured support object
	rcl_ret_t rc2 = rclc_node_init_default(&node, node_name, namespace, &support);
	while (rc2 != RCL_RET_OK) {
		for(int i=0;i<10;i++){
			HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);
			osDelay(500);
		}
		rc2 = rclc_node_init_default(&node, node_name, namespace, &support);
	}


//  //time sync
//  if( rmw_uros_sync_session(1000) != RMW_RET_OK)
//	  printf("Error on time sync (line %d)\n", __LINE__);

  rclc_publisher_init_default(
   	&joint_state_pub,
   	&node,
 	ROSIDL_GET_MSG_TYPE_SUPPORT(actuator_msgs,msg,Actuators),
   	"/X3/gazebo/command/motor_speed");

    float a_velocity[] = {500,500,100,100};
    setActuatorMsg(a_velocity);
  /* Infinite loop */
  for(;;)
  {

	  osDelay(1000);
	  sendActuatorMsg();
	  //joint_state_msg.data = i;
  }
  /* USER CODE END microROSTaskFunction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/*
 * Função responsável por definir o formato da mensagem do atuator.
 * Essa mensagem tem um header composoto por
	 * header : { stamp: {sec:0,nanosec:0}, frame_id:''}
	 * position: []
	 * velocity: []
	 * normalized: []
 * Só nos interessa a velocidade. Então o argumento da função é um ponteiro de float com 4 posições
 */
void setActuatorMsg(float *fpVelocity){
	joint_state_msg.header.frame_id.capacity = 20;
	joint_state_msg.header.frame_id.data = (char*) pvPortMalloc(joint_state_msg.header.frame_id.capacity  * sizeof(char));
	joint_state_msg.header.frame_id.size = strlen(joint_state_msg.header.frame_id.data);


	joint_state_msg.velocity.capacity = 4;
	joint_state_msg.velocity.data = (double*) pvPortMalloc(joint_state_msg.velocity.capacity * sizeof(double));
	joint_state_msg.velocity.data[0] = fpVelocity[0];
	joint_state_msg.velocity.data[1] = fpVelocity[1];
	joint_state_msg.velocity.data[2] = fpVelocity[2];
	joint_state_msg.velocity.data[3] = fpVelocity[3];
	joint_state_msg.velocity_data[4] = isDataNew;
	joint_state_msg.velocity.size = 4;
}

void sendActuatorMsg(){
	rcl_ret_t ret = rcl_publish(&joint_state_pub, &joint_state_msg, NULL);
	if (ret != RCL_RET_OK)
	{
			  printf("Error publishing joint_state (line %d)\n", __LINE__);
	}

}
/* USER CODE END Application */

