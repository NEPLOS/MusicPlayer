all:
	g++ *.cpp *.c \
	`pkg-config --cflags --libs gtk+-3.0 ayatana-appindicator3-0.1` \
	-ldl -lX11 -lleif -lglfw -lm -lGL -lclipboard -lxcb -lSDL2 -lSDL2_mixer -ltag -O3 -pthread -o ff
	./ff


#
#
#all:
#	g++ *.cpp *.c -ldl -lX11 -lleif -lglfw -lm -lGL -lclipboard -lxcb -lSDL2 -lSDL2_mixer -ltag -O3 -o ff
#	./ff
