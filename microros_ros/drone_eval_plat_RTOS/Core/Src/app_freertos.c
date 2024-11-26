/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stdio.h"
#include "string.h"

#include "gpio.h"
#include "usart.h"
#include "tim.h"

#include "MPU6050.h"
#include "CalculateAngle.h"


#include "my_microROS_functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */
#define MAX_MESSAGE_LEN 32
typedef struct {
  int iMessageLen;
  char pMessageBuffer[MAX_MESSAGE_LEN];
} xPrintfMessage;

typedef struct {
  float fYaw;
  float fRoll;
  float fPitch;
  int iDataFromJoystick;
} xSetpoint;

typedef struct {
  float fYaw;
  float fRoll;
  float fPitch;
} xIMUData;

typedef xIMUData xJoystickData;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_OSDELAY_LOOP 4
#define UART_BUFFER_SIZE 256
#define STEP_MOTOR_MICRODELAY 500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// matrix multiplication, we pass ret=A*x, wheere x is 3x1 and returns a 4x1 matrix: A is 4x3
#define MATRIX_MULTIPLICATION(ret, A, x) \
  ret[0] = A[0][0]*x[0] + A[0][1]*x[1] + A[0][2]*x[2]; \
  ret[1] = A[1][0]*x[0] + A[1][1]*x[1] + A[1][2]*x[2]; \
  ret[2] = A[2][0]*x[0] + A[2][1]*x[1] + A[2][2]*x[2]; \
  ret[3] = A[3][0]*x[0] + A[3][1]*x[1] + A[3][2]*x[2];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
xSetpoint xSetpointData = {0, 0, 0, 0};
xSetpoint xHostData = {0, 0, 0, 0};
xJoystickData xJoystickDataIncoming = {0, 0, 0};
float fMotorSpeeds[4] = {500, 500, 500, 500};

unsigned char ucUartInputChar = '\0';
unsigned short int usBufferIndex = 0;
unsigned char ucUartInputBuffer[UART_BUFFER_SIZE];
unsigned short int usLastPrintedIndex = 0;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 3000 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BT_uart */
osThreadId_t BT_uartHandle;
const osThreadAttr_t BT_uart_attributes = {
  .name = "BT_uart",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for printGateKeeper */
osThreadId_t printGateKeeperHandle;
const osThreadAttr_t printGateKeeper_attributes = {
  .name = "printGateKeeper",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for writeSetpoint */
osThreadId_t writeSetpointHandle;
const osThreadAttr_t writeSetpoint_attributes = {
  .name = "writeSetpoint",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for readFromHost */
osThreadId_t readFromHostHandle;
const osThreadAttr_t readFromHost_attributes = {
  .name = "readFromHost",
  .priority = (osPriority_t) osPriorityNormal6,
  .stack_size = 500 * 4
};
/* Definitions for readFromIMU */
osThreadId_t readFromIMUHandle;
const osThreadAttr_t readFromIMU_attributes = {
  .name = "readFromIMU",
  .priority = (osPriority_t) osPriorityRealtime1,
  .stack_size = 128 * 4
};
/* Definitions for updateControl */
osThreadId_t updateControlHandle;
const osThreadAttr_t updateControl_attributes = {
  .name = "updateControl",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 128 * 4
};
/* Definitions for convertSetpoint */
osThreadId_t convertSetpointHandle;
const osThreadAttr_t convertSetpoint_attributes = {
  .name = "convertSetpoint",
  .priority = (osPriority_t) osPriorityBelowNormal5,
  .stack_size = 128 * 4
};
/* Definitions for sendToHost */
osThreadId_t sendToHostHandle;
const osThreadAttr_t sendToHost_attributes = {
  .name = "sendToHost",
  .priority = (osPriority_t) osPriorityNormal5,
  .stack_size = 128 * 4
};
/* Definitions for moveYawMotor */
osThreadId_t moveYawMotorHandle;
const osThreadAttr_t moveYawMotor_attributes = {
  .name = "moveYawMotor",
  .priority = (osPriority_t) osPriorityBelowNormal2,
  .stack_size = 128 * 4
};
/* Definitions for moveRollMotor */
osThreadId_t moveRollMotorHandle;
const osThreadAttr_t moveRollMotor_attributes = {
  .name = "moveRollMotor",
  .priority = (osPriority_t) osPriorityBelowNormal1,
  .stack_size = 128 * 4
};
/* Definitions for printfQueue */
osMessageQueueId_t printfQueueHandle;
const osMessageQueueAttr_t printfQueue_attributes = {
  .name = "printfQueue"
};
/* Definitions for yawMotorNewAngle */
osMessageQueueId_t yawMotorNewAngleHandle;
const osMessageQueueAttr_t yawMotorNewAngle_attributes = {
  .name = "yawMotorNewAngle"
};
/* Definitions for rollMotorNewAngle */
osMessageQueueId_t rollMotorNewAngleHandle;
const osMessageQueueAttr_t rollMotorNewAngle_attributes = {
  .name = "rollMotorNewAngle"
};
/* Definitions for IMUdata */
osMessageQueueId_t IMUdataHandle;
const osMessageQueueAttr_t IMUdata_attributes = {
  .name = "IMUdata"
};
/* Definitions for IMUshouldUpdate */
osSemaphoreId_t IMUshouldUpdateHandle;
const osSemaphoreAttr_t IMUshouldUpdate_attributes = {
  .name = "IMUshouldUpdate"
};
/* Definitions for hostImuEvent */
osEventFlagsId_t hostImuEventHandle;
const osEventFlagsAttr_t hostImuEvent_attributes = {
  .name = "hostImuEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void microDelay(uint16_t delay);
void step(int steps, uint8_t direction, uint16_t delay, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/* USER CODE END FunctionPrototypes */

void defaultTaskFunc(void *argument);
void BT_uart_func(void *argument);
void printGateKeeperFunc(void *argument);
void writeSetpointFunc(void *argument);
void readFromHostFunc(void *argument);
void readFromIMUFunc(void *argument);
void updateControlFunc(void *argument);
void convertSetpointToStepsFunc(void *argument);
void sendToHostFunc(void *argument);
void moveYawMotorFunc(void *argument);
void moveRollMotorFunc(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of IMUshouldUpdate */
  IMUshouldUpdateHandle = osSemaphoreNew(1, 1, &IMUshouldUpdate_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  HAL_TIM_Base_Start(&htim1);
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of printfQueue */
  printfQueueHandle = osMessageQueueNew (128, sizeof(xPrintfMessage), &printfQueue_attributes);

  /* creation of yawMotorNewAngle */
  yawMotorNewAngleHandle = osMessageQueueNew (16, sizeof(float), &yawMotorNewAngle_attributes);

  /* creation of rollMotorNewAngle */
  rollMotorNewAngleHandle = osMessageQueueNew (16, sizeof(float), &rollMotorNewAngle_attributes);

  /* creation of IMUdata */
  IMUdataHandle = osMessageQueueNew (4, sizeof(xIMUData), &IMUdata_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(defaultTaskFunc, NULL, &defaultTask_attributes);

  /* creation of BT_uart */
  BT_uartHandle = osThreadNew(BT_uart_func, NULL, &BT_uart_attributes);

  /* creation of printGateKeeper */
  printGateKeeperHandle = osThreadNew(printGateKeeperFunc, NULL, &printGateKeeper_attributes);

  /* creation of writeSetpoint */
  writeSetpointHandle = osThreadNew(writeSetpointFunc, NULL, &writeSetpoint_attributes);

  /* creation of readFromHost */
  readFromHostHandle = osThreadNew(readFromHostFunc, NULL, &readFromHost_attributes);

  /* creation of readFromIMU */
  readFromIMUHandle = osThreadNew(readFromIMUFunc, NULL, &readFromIMU_attributes);

  /* creation of updateControl */
  updateControlHandle = osThreadNew(updateControlFunc, NULL, &updateControl_attributes);

  /* creation of convertSetpoint */
  convertSetpointHandle = osThreadNew(convertSetpointToStepsFunc, NULL, &convertSetpoint_attributes);

  /* creation of sendToHost */
  sendToHostHandle = osThreadNew(sendToHostFunc, NULL, &sendToHost_attributes);

  /* creation of moveYawMotor */
  moveYawMotorHandle = osThreadNew(moveYawMotorFunc, NULL, &moveYawMotor_attributes);

  /* creation of moveRollMotor */
  moveRollMotorHandle = osThreadNew(moveRollMotorFunc, NULL, &moveRollMotor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of hostImuEvent */
  hostImuEventHandle = osEventFlagsNew(&hostImuEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_defaultTaskFunc */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_defaultTaskFunc */
void defaultTaskFunc(void *argument)
{
  /* USER CODE BEGIN defaultTaskFunc */
  
  /* Infinite loop */
  for(;;)
  {
	//uint32_t valor_flag = osThreadFlagsSet(readFromHostHandle, 0x01);
	//osEventFlagsSet(hostImuEventHandle,0x01);
    vMyMicroros();
    osDelay(1000);
  }
  /* USER CODE END defaultTaskFunc */
}

/* USER CODE BEGIN Header_BT_uart_func */
/**
* @brief Function implementing the BT_uart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BT_uart_func */
void BT_uart_func(void *argument)
{
  /* USER CODE BEGIN BT_uart_func */

  /* Infinite loop */
  printf("\r\nbluetooth IS on\r\n");
  for(;;)
  {
    HAL_UART_Receive_IT(&huart5, &ucUartInputChar, 1);

    // If buffer size differs from printed amount, print until they match
    while ( usLastPrintedIndex != usBufferIndex ){

      printf("bluetooth int: ");
      if ( ucUartInputBuffer[usLastPrintedIndex] == '0')
        printf("KEY_RELEASED");
      else
        printf((char *) &( ucUartInputBuffer[usLastPrintedIndex] ));
      usLastPrintedIndex++;


      printf("\r\n");
      if (usLastPrintedIndex >  UART_BUFFER_SIZE - 1)
        usLastPrintedIndex = 0;
    }
    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END BT_uart_func */
}

/* USER CODE BEGIN Header_printGateKeeperFunc */
/**
* @brief Function implementing the printGateKeeper thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_printGateKeeperFunc */
void printGateKeeperFunc(void *argument)
{
  /* USER CODE BEGIN printGateKeeperFunc */
  xPrintfMessage xIncommingMessage;
  /* Infinite loop */
  for(;;)
  {

    if (osMessageQueueGet(printfQueueHandle, &xIncommingMessage, 0x0, 10) == osOK){
    	if(HAL_UART_Transmit(&huart3,(uint8_t *)xIncommingMessage.pMessageBuffer, xIncommingMessage.iMessageLen, 100) != HAL_OK){
      //if(HAL_UART_Transmit(&hlpuart1,(uint8_t *)xIncommingMessage.pMessageBuffer, xIncommingMessage.iMessageLen, 100) != HAL_OK){
        Error_Handler();
      }
    }
    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END printGateKeeperFunc */
}

/* USER CODE BEGIN Header_writeSetpointFunc */
/**
* @brief Function implementing the writeSetpoint thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_writeSetpointFunc */
void writeSetpointFunc(void *argument)
{
  /* USER CODE BEGIN writeSetpointFunc */
  uint32_t uiThreadFlagsReturn = 0;

  /* Infinite loop */
  for(;;)
  {
    // wait for writeSetpointFunc task flag to be different from 0
    uiThreadFlagsReturn = osThreadFlagsWait(0b11, osFlagsWaitAny, 1000);
    // if return is 0x1, read data from joystick
    // if return is 0x10, read data from host
    // else we loop and wait for the flag to be set
    if (uiThreadFlagsReturn == 0b01){
      // read data from joystick
      xSetpointData.iDataFromJoystick = 1;
      xSetpointData.fYaw = xJoystickDataIncoming.fYaw;
      xSetpointData.fRoll = xJoystickDataIncoming.fRoll;
      xSetpointData.fPitch = xJoystickDataIncoming.fPitch;
      
    }
    else if (uiThreadFlagsReturn == 0b10){
      // read data from host
      xSetpointData.iDataFromJoystick = 0;
      xSetpointData.fYaw = xHostData.fYaw;
      xSetpointData.fRoll = xHostData.fRoll;
      xSetpointData.fPitch = xHostData.fPitch;
      //osThreadFlagsSet(updateControlHandle,0x01);

    }
    osDelay(4);
  }
  /* USER CODE END writeSetpointFunc */
}

/* USER CODE BEGIN Header_readFromHostFunc */
/**
* @brief Function implementing the readFromHost thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_readFromHostFunc */
void readFromHostFunc(void *argument)
{
  /* USER CODE BEGIN readFromHostFunc */
    // IVAN CODE HERE <--------------------------------------------------------------------------------------------------------
	xSetpoint xHostData = {0, 0, 0, 0};
	int i=0;
  /* Infinite loop */
  for(;;)
  {
    // IVAN CODE HERE <--------------------------------------------------------------------------------------------------------
	osEventFlagsWait(hostImuEventHandle, 0x01, osFlagsWaitAll, osWaitForever);
	HAL_GPIO_TogglePin(LD2_GPIO_Port , LD2_Pin);
	float a_velocity[] = {0+i,10+i,100+i,500+i};
	vSetActuatorMsg(a_velocity);
	//vSendActuatorMsg();
	//osThreadFlagsSet(writeSetpointHandle,0b10);
	osDelay(DEFAULT_OSDELAY_LOOP);
	i++;
	i%=100;
  }
  /* USER CODE END readFromHostFunc */
}

/* USER CODE BEGIN Header_readFromIMUFunc */
/**
* @brief Function implementing the readFromIMU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_readFromIMUFunc */
void readFromIMUFunc(void *argument)
{
  /* USER CODE BEGIN readFromIMUFunc */
  
  MPU6050_Initialization();
  /* Infinite loop */
  for(;;)
  {
    // IMUshouldUpdate semaphore is set to 1 when the IMU has new data
    if (osSemaphoreAcquire(IMUshouldUpdateHandle, 0) == osOK){
      MPU6050_ProcessData(&MPU6050);
      CalculateCompliFilter(&Angle, &MPU6050);
      // printf("%f, %f, %f\r\n", Angle.ComFilt_roll,Angle.ComFilt_pitch,Angle.ComFilt_yaw);
    }
    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END readFromIMUFunc */
}

/* USER CODE BEGIN Header_updateControlFunc */
/**
* @brief Function implementing the updateControl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_updateControlFunc */
void updateControlFunc(void *argument)
{
  /* USER CODE BEGIN updateControlFunc */
  xIMUData xIMUDataIncoming = {0, 0, 0};
  float xError[] = {0, 0, 0};
  xIMUData xIMUDataPrevious = {0, 0, 0};
  xIMUData xIMUDataErrorAccumulated = {0, 0, 0};
  float fMotorSpeedsDiff[4] = {0, 0, 0, 0};

  // we need a matrix to map the error to the motor speeds
  float fStateToMotorSpeedMatrix[4][3] = {
    {1, 1, 1},
    {1, -1, -1},
    {-1, -1, 1},
    {-1, 1, -1}
  };

  /* Infinite loop */
  for(;;)
  {

    if (osMessageQueueGet(IMUdataHandle, &xIMUDataIncoming, 0x0, 100) == osOK){
      xError[0] = xSetpointData.fYaw - xIMUDataIncoming.fYaw;
      xError[1] = xSetpointData.fRoll - xIMUDataIncoming.fRoll;
      xError[2] = xSetpointData.fPitch - xIMUDataIncoming.fPitch;

      xIMUDataErrorAccumulated.fYaw += xError[0];
      xIMUDataErrorAccumulated.fRoll += xError[1];
      xIMUDataErrorAccumulated.fPitch += xError[2];

      MATRIX_MULTIPLICATION(fMotorSpeedsDiff, fStateToMotorSpeedMatrix, xError);
      fMotorSpeeds[0] = fMotorSpeeds[0] + fMotorSpeedsDiff[0];
      fMotorSpeeds[1] = fMotorSpeeds[1] + fMotorSpeedsDiff[1];
      fMotorSpeeds[2] = fMotorSpeeds[2] + fMotorSpeedsDiff[2];
      fMotorSpeeds[3] = fMotorSpeeds[3] + fMotorSpeedsDiff[3];

      // we need to update the real motor angles that will move the IMU
      osMessageQueuePut(yawMotorNewAngleHandle, & xIMUDataIncoming.fYaw, 0, 1);
      osMessageQueuePut(rollMotorNewAngleHandle, & xIMUDataIncoming.fRoll, 0, 1);

      // we need to update the motor speeds to micro
      osThreadFlagsSet(sendToHostHandle, 0x1);

      xIMUDataPrevious = xIMUDataIncoming;
      (void) xIMUDataPrevious; // just use to avoid warning, but would be necessary at PID
    }

//    if(0x01 == osThreadFlagsWait(0x01, osFlagsWaitAny, 1000)){
//    	static int i = 0;
//    	fMotorSpeeds[0] = i;
//    	fMotorSpeeds[1] = i+10;
//    	fMotorSpeeds[2] = i+100;
//    	fMotorSpeeds[3] = i+500;
//    	osThreadFlagsSet(sendToHostHandle, 0x1);
//    	i++;
//    	i%=100;
//    }

    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END updateControlFunc */
}

/* USER CODE BEGIN Header_convertSetpointToStepsFunc */
/**
* @brief Function implementing the convertSetpoint thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_convertSetpointToStepsFunc */
void convertSetpointToStepsFunc(void *argument)
{
  /* USER CODE BEGIN convertSetpointToStepsFunc */
  // we know that  25 steps equals 45 degrees of the motor
  // a full rotation is 45 degrees * 8 = 360 degrees
  // but the gear diameter ratio is 1.5cm at motor to 8cm at IMU
  float fGearDiameterRatio = 8/1.5;
  float fMotorStepsToIMUFullRotation = 25 * fGearDiameterRatio;
  xIMUData xIMUDataPrevious = {0, 0, 0};
  float fNewAngleYaw = 0;
  float fNewAngleRoll = 0;


  /* Infinite loop */
  for(;;)
  {
    // wait thread flag to be set to 1
    if (osThreadFlagsWait(0x1, 0x11, 1000) == 0x1){
      // amount of steps to move the motor is the difference between the new angle and the previous angle
      // divided by the amount of steps to move the IMU a full rotation
      // we add to rollMotorNewAngleHandle and yawMotorNewAngleHandle
      fNewAngleYaw = (xSetpointData.fYaw - xIMUDataPrevious.fYaw) / fMotorStepsToIMUFullRotation;
      fNewAngleRoll = (xSetpointData.fRoll - xIMUDataPrevious.fRoll) / fMotorStepsToIMUFullRotation;
      osMessageQueuePut(yawMotorNewAngleHandle, & fNewAngleYaw, 0, 100);
      osMessageQueuePut(rollMotorNewAngleHandle, & fNewAngleRoll, 0, 100);

      xIMUDataPrevious.fYaw = xSetpointData.fYaw;
      xIMUDataPrevious.fRoll = xSetpointData.fRoll;
      xIMUDataPrevious.fPitch = xSetpointData.fPitch;
    }
    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END convertSetpointToStepsFunc */
}

/* USER CODE BEGIN Header_sendToHostFunc */
/**
* @brief Function implementing the sendToHost thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sendToHostFunc */
void sendToHostFunc(void *argument)
{
  /* USER CODE BEGIN sendToHostFunc */
  // IVAN CODE HERE <--------------------------------------------------------------------------------------------------------
/* Infinite loop */
  for(;;)
  {
    // if thread flag is set to 1, send data to host
    if (osThreadFlagsWait(0x1, osFlagsWaitAny, 1000) == 0x1){
      // IVAN CODE HERE <--------------------------------------------------------------------------------------------------------
		vSetActuatorMsg(fMotorSpeeds);
    	vSendActuatorMsg();
    }
    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END sendToHostFunc */
}

/* USER CODE BEGIN Header_moveYawMotorFunc */
/**
* @brief Function implementing the moveYawMotor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_moveYawMotorFunc */
void moveYawMotorFunc(void *argument)
{
  /* USER CODE BEGIN moveYawMotorFunc */
  float fNewSteps;
  /* Infinite loop */
  for(;;)
  {
    if (osMessageQueueGet(yawMotorNewAngleHandle, &fNewSteps, 0x0, 100) == osOK){
      step(fNewSteps, fNewSteps>0?1:0, STEP_MOTOR_MICRODELAY, M2_pin2_GPIO_Port, M2_pin2_Pin);
    }

    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END moveYawMotorFunc */
}

/* USER CODE BEGIN Header_moveRollMotorFunc */
/**
* @brief Function that moves the roll motor
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_moveRollMotorFunc */
void moveRollMotorFunc(void *argument)
{
  /* USER CODE BEGIN moveRollMotorFunc */
  float fNewSteps;
  /* Infinite loop */
  for(;;)
  {
    if (osMessageQueueGet(rollMotorNewAngleHandle, &fNewSteps, 0x0, 100) == osOK){
      step(fNewSteps, fNewSteps>0?1:0, STEP_MOTOR_MICRODELAY, M1_pin2_GPIO_Port, M1_pin2_Pin);
    }

    osDelay(DEFAULT_OSDELAY_LOOP);
  }
  /* USER CODE END moveRollMotorFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// *****************************************************    //
// Method name: HAL_UART_RxCpltCallback                     //
// Method description:  This method overrides the UART      //
//                      detection callback, which is        //
//                      triggered when a new char arrives   //
//                      It sends the char to the cyclic     //
//                      buffer and updates the buffer index //
// Input params:        huart                               //
//                      The specific uart that              //
//                      triggered the interrupt.            //
// Output params:       n/a                                 //
// *****************************************************    //
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart5){
    ucUartInputBuffer[usBufferIndex++] = ucUartInputChar;
    if(usBufferIndex > UART_BUFFER_SIZE - 1)
      usBufferIndex = 0;
  }
}

// *********************************************************//
// Method name: step                                        //
// Method description:  This method moves a given motor     //
//                      a given amount of steps in a given  //
//                      direction.                          //
// Input params:        steps                               //
//                      The amount of steps to move the     //
//                      motor.                              //
//                      direction                           //
//                      The direction to move the motor     //
//                      1 for clockwise, 0 for counter      //
//                      clockwise.                          //
//                      delay                               //
//                      The delay between each step.        //
//                      GPIOx                               //
//                      The GPIO port of the motor.         //
//                      GPIO_Pin                            //
//                      The GPIO pin of the motor.          //
// Output params:       n/a                                 //
// *********************************************************//
void step(int steps, uint8_t direction, uint16_t delay, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  int x;

  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, !direction);
  for(x=0; x<steps; x=x+1)
  {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
    microDelay(delay);
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
    microDelay(delay);
  }
}

// *********************************************************//
// Method name: microDelay                                  //
// Method description:  This method creates a delay in      //
//                      microseconds.                       //
// Input params:        delay                               //
//                      The delay in microseconds.          //
// Output params:       n/a                                 //
// *********************************************************//
void microDelay (uint16_t delay)
{
  int a = 0;
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  do{
    a = __HAL_TIM_GET_COUNTER(&htim1);
  }while (a < delay);
}


// *********************************************************//
// Method name: _write                                      //
// Method description:  This method overrides the write     //
//                      method, which is used by printf.    //
//                      It sends the message to the         //
//                      printfQueue.                        //
// Input params:        file                                //
//                      The file to write to.               //
//                      ptr                                 //
//                      The pointer to the message.         //
//                      len                                 //
//                      The length of the message.          //
// Output params:       len                                 //
//                      The length of the message.          //
// *********************************************************//
int _write(int file, char *ptr, int len)
{
  xPrintfMessage xIncommingMessage;

  len = MIN(len, MAX_MESSAGE_LEN)+1; //+1 to '\0'
  xIncommingMessage.iMessageLen = len;
  strncpy(xIncommingMessage.pMessageBuffer, ptr, len);
  xIncommingMessage.pMessageBuffer[len] = '\0';
  osMessageQueuePut(printfQueueHandle, &xIncommingMessage, 0x0, 100);
  return len;
}


/* USER CODE END Application */

