#! /bin/bash


# Este arquivo é resposnsável pelas estilizações, isto é teste em cores
# Indicar o final de operações e etc.

#Colors for text
BBlack='\033[1;30m'       # Black
BYellow='\033[1;33m'        # Blue
BPurple='\033[1;35m'      # Purple
BRed='\033[1;31m'         # Red
Color_Off='\033[0m'       # No color
BGreen='\033[1;32m'       # Green
BCyan='\033[1;36m'        # Cyan
On_Black='\033[40m'       # Black background
equal_char="="

init_style(){
    repeated_char=$(printf "$equal_char%.0s" $(seq 1 150))
}

normal_word(){
    echo -e $2 ${Color_Off}$1
}

red_word () {
    echo -e $2 ${BRed}$1${Color_Off}
}

black_word () {
    echo -e $2 ${BBlack}$1${Color_Off}
}

green_word () {
    echo -e $2 ${BGreen}$1${Color_Off}
}

yellow_word () {
    echo -e $2 ${BYellow}$1${Color_Off}
}

purple_word () {
    echo -e $2 ${BPurple}$1${Color_Off}
}

stage_over (){
	echo  -e "${On_Black}$repeated_char${Color_Off}"
	echo -e "${On_Black}$repeated_char${Color_Off}\n"
}

stage_init(){
    echo -e $2 ${BYellow}$1"\n"${Color_Off} 
    #echo -e "${On_Black}$repeated_char${Color_Off}\n"
}

sub_stage_init(){
    echo -e $2 "\t"${BYellow}$1"\n"${Color_Off} 
    #echo -e "${On_Black}$repeated_char${Color_Off}\n"
}


export -f red_word