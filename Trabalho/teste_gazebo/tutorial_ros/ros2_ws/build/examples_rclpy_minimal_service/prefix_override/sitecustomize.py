import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_gazebo/tutorial_ros/ros2_ws/install/examples_rclpy_minimal_service'
