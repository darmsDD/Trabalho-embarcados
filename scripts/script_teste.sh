#! /bin/bash


set -a
export folder_name="banana"
export stm_workspace_name="microros_ros"
export path_utils="microros_ros/drone_eval_plat_RTOS/"
export micro_utils_name="microros_ros/drone_eval_plat_RTOS/micro_ros_stm32cubemx_utils"
export my_ros_domain_id=25
export clone_command="git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup"
export topic_velocity_name="/X3/gazebo/command/motor_speed"
export actuator_ros_message_type="actuator_msgs/msg/Actuators"
export actuator_gazebo_msg_type="gz.msgs.Actuators"
export imu_ros_message_type="sensor_msgs/msg/Imu"
export imu_gazebo_msg_type="gz.msgs.IMU"
export topic_imu_name="drone/imu"
export session_name=micro_ros
export window_name=window1
export full_path="$session_name:$window_name"


. ./style.sh
init_style
. ./base_functions.sh
. ./microROS_functions.sh

trap terminateProgram SIGINT

stage_init "0- Procurando o diretório $micro_utils_name"
ExecuteFunctionAndCheckError FindMicroUtils
stage_over

stage_init "1- Procurando o diretório $folder_name"
ExecuteFunctionAndCheckError FindDir RmCreatedDir
stage_over

stage_init "2- Realizando a build e instalando as dependências"
ExecuteFunctionAndCheckError InitialSetup RmCreatedDir
stage_over

stage_init "3- Criando o agente do micro_ros"
ExecuteFunctionAndCheckError MicrosRosAgentSetup RmCreatedDir
stage_over

stage_init "4- Iniciando o gazebo"
StartGazebo
stage_over

stage_init "5- Iniciando a STM32CubeIDE"
sub_stage_init "5.1- Buildando o projeto"
BuildStm32CubeProject
StartStm32CubeIde
sub_stage_init "5.2- Abrindo o STM32CubeIDE"
stage_over

stage_init "6- Configurando o tmux com 3 painéis:\n\t1.Agente do MicroRos\n\t2.Microcontrolador => PC \n\t3.PC =>  Microcontrolador"

sub_stage_init "6.1- Criando a sessão tmux"
ExecuteFunctionAndCheckError TmuxCreateSession TmuxKillSession
sub_stage_init "6.2- Estilizando a sessão tmux"
ExecuteFunctionAndCheckError TmuxStyleSession
sub_stage_init "6.3- Iniciando os painéis"
ExecuteFunctionAndCheckError TmuxStartPaneCommands TmuxKillSession
stage_over
stage_init "Finalizando o programa"

ExecuteFunctionAndCheckError TmuxKillSession

# # #gnome-terminal -- bash -c 'tmux send-keys -t $session_name:$window_name.0 "echo "arroz"" Enter'
kill_process_tree $$ # o gazebo cria vários processos descendentes, então para matar todos foi necessário uma árvore
wait # ## Espera os processos finalizarem
set +a