all:
	g++ *.cpp *.c -ldl -lX11 -lleif -lglfw -lm -lGL -lclipboard -lxcb -lSDL2 -lSDL2_mixer -ltag -O3 -o ff
	./ff