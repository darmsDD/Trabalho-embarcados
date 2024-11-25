#! /bin/bash


# export topic_velocity_name="/X3/gazebo/command/motor_speed"
# export actuator_ros_message_type="actuator_msgs/msg/Actuators"
# export actuator_gazebo_msg_type="gz.msgs.Actuators"
# export imu_ros_message_type="sensor_msgs/msg/Imu"
# export imu_gazebo_msg_type="gz.msgs.IMU"
# export topic_imu_name="drone/imu"
# export my_ros_domain_id=25

# Faz a build do pacote e instala dependências 
InitialSetup() {
    rosdep install --from-path src --ignore-src -y
    if [ -d "src" ] && [ -d "build" ] && [ -d "install" ] && [ -d "log" ]; then
        purple_word "Build já realizada anteriormente. Pulando esta etapa."
    else
        colcon build
        colcon test
    fi
    source install/local_setup.bash
}


# Executa os scripts necessários para o agente do micro_ros funcionar
MicrosRosAgentSetup(){
    if [ -f src/ros2.repos ]
    then
        purple_word "Setup já realizado anteriormente. Pulando esta etapa."
    else
        ros2 run micro_ros_setup create_agent_ws.sh
        ros2 run micro_ros_setup build_agent.sh    
    fi 
}

# Roda o agente do micro-ros, permitindo observar no terminal com mais detalhes os processos.
# Como por exemplo, ver quem se inscreveu.
MicrosRosAgentRun(){
    source install/local_setup.bash 
    ros2 run micro_ros_agent micro_ros_agent serial --dev $(ls /dev/serial/by-id/*) -b 115200
}

# Função utilizada para checar se o tópico ROS está disponível, e se não estiver, avisar o usuário o que ele pode fazer.
IsTopicAvaible(){
    export ROS_DOMAIN_ID=$my_ros_domain_id
    local topic_name=$1
    purple_word "Rode o código no microcontrolador ou aperte o botão de reset, caso o código já esteja na placa."
    ros2 topic echo $topic_name
    while [[ $? -ne 0 ]]; do
        red_word "Tópico ainda não está disponível, rode o código do microcontrolador. Esperarei 5 segundos e tentarei novamente."
        sleep 5
        ros2 topic echo $topic_name
    done
}

# Função utilizada para observar no terminal as mensagens sendo enviadas pelo micro-ros, isto é, o Target (Microcontrolador) para o o Host (PC).
RosSubscriber(){
    IsTopicAvaible $topic_velocity_name
}

# Função utilizada para observar no terminal as mensagens sendo enviadas pelo gazebo, isto é, o Host (PC) para o Target (Microcontrolador).
RosPublisher(){
    IsTopicAvaible $topic_imu_name
}

# Função utilizada para transformar as mensagens to tipo ros para gazebo e gazebo para ros. Permitindo a comunicação entre os tópicos.
RosBridge(){
    export ROS_DOMAIN_ID=$my_ros_domain_id
    bridge_actuator=$topic_velocity_name@$actuator_ros_message_type@$actuator_gazebo_msg_type 
    bridge_imu=$topic_imu_name@$imu_ros_message_type@$imu_gazebo_msg_type
    ros2 run ros_gz_bridge parameter_bridge $bridge_actuator $bridge_imu &
}

# Função utilizada para iniciar a simulação do gazebo em segundo plano.
# -r: Serve para dar autoplay na simulação.
StartGazebo(){
    gz sim ../../models/quadcopter.sdf -r &
}


# if [ -z "$1" ];  then
#     InitialSetup
#     MicrosRosAgentSetup
#     MicrosRosAgentRun
# else
#     RosSubscriber
# fi



# Exporta as funções do micro-ros para serem usadas pelo tmux (um tipo de terminal).
export -f MicrosRosAgentRun
export -f RosSubscriber
export -f RosPublisher
export -f RosBridge
#gnome-terminal -- bash -c "'echo hello world; sleep 3';${BBlue} read -n1 -p ' Press any key to exit.'"
#gnome-terminal --tab --title="test" --command="bash -c '${BBlue} echo 'estou em outra aba'; $SHELL'"