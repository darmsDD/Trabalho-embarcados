# SimuladorDrone
Simulador de um drone com microROS e gazebo

## Passos para executar esse programa:

1. Acesse a pasta ```microros_ros/scripts``` pelo terminal.
2. Execute ``` ./script_teste.sh ```. 
3. Serão executadas diversas etapas pelo script. Inclusive a abertura da aplicação STM32CubeIDE.
    1. Se o projeto já tiver sido adicionado ao workspace "microros_ros" a build será feita pelo script. 
Caso contrário é necessário realizar a build pela própria interface da aplicação STM32CubeIDE.
    2. Execute o código, pelo STM32CubeIDE.
    3. Depois de rodar o código na placa, o STM32CubeIDE pode ser fechado.
5. Para finalizar aperte ctrl+b e depois a tecla d. Isso fechará a sessão tmux, a simulação e o STM32CubeIDE (se não tiver sido fechado anteriormente).
