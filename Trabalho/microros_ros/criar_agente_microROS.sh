#! /bin/bash

# Faz a build do pacote e instala dependências 
InitialSetup() {
    rosdep install --from-path src --ignore-src -y
    colcon build
    source install/local_setup.bash
}


# Executa os scripts necessários para o agente do micro_ros funcionar
MicrosRosAgentSetup(){
    ros2 run micro_ros_setup create_agent_ws.sh
    ros2 run micro_ros_setup build_agent.sh   
}

# Roda o agente do micro-ros, permitindo observar no terminal com mais detalhes os processos.
# Como por exemplo, ver quem se inscreveu.
MicrosRosAgentRun(){
    source install/local_setup.bash 
    ros2 run micro_ros_agent micro_ros_agent serial --dev $(ls /dev/serial/by-id/*) -b 115200
}


# Função utilizada para observar no terminal as mensagens sendo enviadas pelo micro-ros, isto é, o Target (Microcontrolador).
RosSubscriber(){
    #source /opt/ros/$ROS_DISTRO/setup.bash
    export ROS_DOMAIN_ID=$my_ros_domain_id
    # Subscribe to micro-ROS joint_state topic
    ros2 topic echo $topic_velocity_name
    while [[ $? -ne 0 ]]; do
        #red_word "Falhou na inscrição. Esperarei 5 segundos e tentarei novamente."
        #red_word "Tópico ainda não está disponível, rode o código do microcontrolador. Esperarei 5 segundos e tentarei novamente.\033[0m"
        sleep 5
        ros2 topic echo $topic_velocity_name
    done

}

# Função utilizada para enviar mensagens do ROS para o micro-ros, isto é, do Host (computador) para o Target (Microcontrolador)
RosPublisher(){
    #source /opt/ros/$ROS_DISTRO/setup.bash
    # Set ROS_DOMAIN_ID with the same value set in micro-ROS (line 239, app_frertos.c)
    export ROS_DOMAIN_ID=$my_ros_domain_id
    # Publish to micro-ROS /cmd_vel geometry_msgs/msg/Twist topic
    ros2 topic pub --rate 1 /cmd_vel geometry_msgs/msg/Twist '{linear: {x: 2.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}'
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
#gnome-terminal -- bash -c "'echo hello world; sleep 3';${BBlue} read -n1 -p ' Press any key to exit.'"
#gnome-terminal --tab --title="test" --command="bash -c '${BBlue} echo 'estou em outra aba'; $SHELL'"