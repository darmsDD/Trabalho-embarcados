#! /bin/bash


set -a
export folder_name=banana
export my_ros_domain_id=25
export clone_command="git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup"
export topic_velocity_name="/X3/gazebo/command/motor_speed"
my_kill_session="tmux kill-session -t $session_name"
. ./style.sh
init_style
. ./base_functions.sh
. ./criar_agente_microROS.sh

stage_init "1- Procurando o diretório $folder_name"
ExecuteFunctionAndCheckError FindDir RmCreatedDir
stage_over

stage_init "2- Realizando a build e instalando as dependências"
ExecuteFunctionAndCheckError InitialSetup
stage_over

stage_init "3- Criando o agente do micro_ros"
ExecuteFunctionAndCheckError MicrosRosAgentSetup
stage_over

stage_init "4- Configurando o tmux com 3 paineis:\n\t1.Agente do MicroRos\n\t2.Microcontrolador => PC \n\t3.PC =>  Microcontrolador"
session_name=micro_ros
window_name=window1
full_path="$session_name:$window_name"

tmux new-session -d -s $session_name
tmux rename-window -t $session_name:0 $window_name                                               

tmux split-window -h
tmux split-window -v

tmux send-keys -t $full_path.0 "MicrosRosAgentRun" Enter
read -p "Aperte enter depois de rodar o código no microcontrolador"
tmux send-keys -t $full_path.1 "RosSubscriber" Enter
tmux send-keys -t $full_path.2 "RosPublisher" Enter

tmux attach -t $session_name
tmux kill-session -t $session_name

# #gnome-terminal -- bash -c 'tmux send-keys -t $session_name:$window_name.0 "echo "arroz"" Enter'
set +a