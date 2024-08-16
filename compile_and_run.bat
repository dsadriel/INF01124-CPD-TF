set main_file=%1

gcc -std=c11 -Wall -I include -L lib -o build\%main_file%.exe lib\* src\%main_file%.c

build\%main_file%.exe
