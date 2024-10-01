main: main.c
	gcc main.c -Lraylib/src/ -l:libraylib.a -lm -o main

