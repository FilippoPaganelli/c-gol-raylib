gol: gol.c
	gcc gol.c -Lraylib/src/ -l:libraylib.a -lm -o gol

