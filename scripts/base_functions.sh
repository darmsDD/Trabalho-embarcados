#! /bin/bash

# Cria a pasta necessária e depois faz um git clone dentro dela.
# Os documentos dentro desse novo repositório são necessários para rodar o agente do micro-ros.
StepsForCreatingDir(){
    mkdir -p $folder_name
    cd $folder_name
    $clone_command
}

# Remove a pasta criada.
RmCreatedDir(){
    cd ..
    rm -rf $folder_name
}


# Executa a função e verifica se houve algum erro. No caso de erro executa a função de limpeza.
# Argumento 1: função a ser executada
# Argumento 2: função de limpeza a ser executada em caso de erro. 
ExecuteFunctionAndCheckError(){
    local my_function=$1 
    local my_clean_up_function=$2
    $my_function # executa a função passada como argumento
    if [[ $? = 0 ]]; then
            green_word "Sucesso"
    else
        # red_word "Aconteceu uma falha inesperada. Verique as funções chamadas na seguinte ordem: $FUNCNAME => $my_function"
        red_word "Um erro pode ter acontecido. Veja a ordem das funções chamadas":
        purple_word "$my_function ${FUNCNAME[*]}"
        $my_clean_up_function # executa a funcao de limpeza no caso de falha (Removendo arquivos e etc que foram criados durante o processo).
        exit $?
    fi
}


# Pergunta se você deseja executar as etapas declaradas anteriormente.
# Em caso positivo, executa a função passada como o primeiro argumento.
# Em caso negativo, executa a função de limpeza passada como o segundo argumento.
# Argumento 1: Função a ser executada
# Argumento 2: Função de limpeza
AuthorizationInput(){
    local my_function=$1
    yellow_word "Você autoriza a realização dessa(s) estapa(s)? [S/n]:" -n
    read input
    if [ $input == "S" ]; then
        #ExecuteFunctionAndCheckError $my_function $my_clean_up_function
        $my_function
    else
        red_word "Autorização não concedida."
        return 1
    fi
    
}


# Cria o diretório que receberá os arquivos do comando "git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup"
CreateDir(){
    purple_word "Diretório não encontrado."
    yellow_word "Para resolver, as seguintes etapas devem ser realizadas:"
    normal_word "\t1-mkdir $folder_name\n\t2-cd $folder_name\n\t3-$clone_command\n" 
    AuthorizationInput StepsForCreatingDir
}

# Responsável por verificar se o diretório com nome $folder_name já existe.
# Se o diretório não existir, ele é criado e depois é clonado o repositório. 
# O comando de clone é definido pela variável $clone_command no início do arquivo script_test.sh.
# A variável $folder_name recebe seu valor no início do arquivo script_test.sh.
FindDir() {
    #yellow_word
    cd ../$stm_workspace_name
    dir=$(find -maxdepth 1 -type d -iname $folder_name)
    
    if [[ -n "$dir" ]]; then
        cd $dir
    else
        CreateDir
    fi
}

# Responsável por criar uma nova sessão do TMUX
TmuxCreateSession(){
    tmux new-session -d -s $session_name  
}

# Responsável por estilizar a sessão do TMUX, renomeando a janela e criando 4 painéis de mesmo tamanho.
TmuxStyleSession(){
    tmux rename-window -t $session_name:0 $window_name
    tmux split-window -v
    tmux split-window -h    
    # tmux select-pane -t 0
    #tmux split-window -v
    #tmux select-layout tiled
}

# Responsável por enviar os comandos corretos para cada painel.
# Painel 1: Roda o agente do microROS.
# Painel 2: Mostra as mensagens que o simulador está enviando para o micro-ros.
# Painel 3: Mostra as mensagens que o microROS está enviando para o simulador.
# Painel 4: Executa o comando de brigde do ROS. Convertendo as mensagens do tipo gazebo para o tipo ROS.
TmuxStartPaneCommands(){
    tmux send-keys -t $full_path.0 "MicrosRosAgentRun" Enter
    #read -p "Aperte enter depois de rodar o código no microcontrolador ou aperte o botão de reset, caso o código já esteja na placa."
    tmux send-keys -t $full_path.1 "RosSubscriber" Enter
    tmux send-keys -t $full_path.2 "RosBridge" Enter
    tmux send-keys -t $full_path.2 "RosPublisher" Enter
    #tmux send-keys -t $full_path.3 "RosBridge" Enter
    tmux attach -t $session_name
}

# Responsável por mata a sessão do TMUX.
TmuxKillSession(){
    tmux kill-session -t $session_name
}

# Responsável por finalizar o programa de maneira correta no caso de um CTRL+C.
# 1- Remove a pasta onde foi clonado o repositório e feito a build.
# 2- Termina a sessão do TMUX.
# 3- Mata os processos filhos. Obs: O gazebo é executado em segundo plano e ele cria uma árvore de processos, então para terminá-lo corretamente, 
# foi necessária uma função que matásse todos os processos descendentes. 
terminateProgram(){
    purple_word "Terminando o programa."
    normal_word "Removendo a pasta $folder_name."
    AuthorizationInput RmCreatedDir
    normal_word "Terminando a sessão do tmux."
    AuthorizationInput TmuxKillSession
    kill_process_tree $$
    wait
    exit
}

# Responsável por eliminar todos os descendentes de um processo pai.
kill_process_tree() {
    local parent_pid=$1
    # Find all child processes
    local child_pids=$(pgrep -P $parent_pid)
    for child_pid in $child_pids; do
        # Recursively kill child processes
        kill_process_tree $child_pid
    done
    # Kill the parent process if it is not the original one
    if [ $parent_pid -ne $$ ]; then
      kill -SIGTERM $parent_pid 2>/dev/null
    fi
}



export -f ExecuteFunctionAndCheckError