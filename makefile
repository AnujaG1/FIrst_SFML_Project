all:compile link
compile:
	g++ -c main.cpp -c Game.cpp -c Bird.cpp -c Pipe.cpp -I"/home/anuja/Videos/SFML-2.6.1/include" -DSFML_STATIC
link:
	g++ main.o Game.o Bird.o Pipe.o -o main -L"/home/anuja/Videos/SFML-2.6.1/lib" -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
clean:
	rm -rf *.o main
