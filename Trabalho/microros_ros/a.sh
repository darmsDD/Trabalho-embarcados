func_a() {
  echo "Function a"
}

func_b() {
  local my_func=$1
  echo "The name of the function is: $my_func"
}

# Call func_b and pass the name of func_a
if [ -z "$1" ]; 
then
    echo "is null"

else
    echo "not null".
fi
func_b func_a
equal_char="="
repeated_char=$(printf "$equal_char%.0s" $(seq 1 100))
echo $repeated_char
echo -e "\033[1;31m Falhou na inscrição. Esperarei 5 segundos e tentarei novamente.\033[0m"