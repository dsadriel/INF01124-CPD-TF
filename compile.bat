set main_file=%1
gcc -std=c11 -Wall -I include -L lib -o %main_file% lib\* src\%main_file%.c