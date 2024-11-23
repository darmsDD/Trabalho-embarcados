func_a() {
  echo "Function a"
}

func_b() {
  local my_func=$1
  echo "The name of the function is: $my_func"
}

t(){
  echo "asdasd"
  kill 0
  echo "whyyyy"
}

meu_c(){
  echo "Passei no sigint"
  exit
}

kill_process_tree() {
    local parent_pid=$1
    # Find all child processes
    local child_pids=$(pgrep -P $parent_pid)
    for child_pid in $child_pids; do
        # Recursively kill child processes
        kill_process_tree $child_pid
    done
    # Kill the parent process
    if [ $parent_pid -ne $$ ]; then
      kill -SIGTERM $parent_pid 2>/dev/null
    fi
}

# Call the function with the parent PID


echo "The PID of this script is $$"
gz sim ../gazebo_ros/models/quadcopter.sdf &
CHILD_PID=$!
echo "Finding child processes of PID $$:"
# func_a
ps --ppid $$
# func_a
# jobs -pr
# func_a
# func_b
# ps -s $$ -o pid=
# func_a
# pstree -p $$ | grep -oP '\(\d+\)' | tr -d '()'
sleep 5
kill_process_tree $$
wait
echo "Eu esperei por você minha princesa"