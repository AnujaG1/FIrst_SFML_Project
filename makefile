all:compile link
compile:
	g++ -c main.cpp -c Game.cpp -c Bird.cpp -c Pipe.cpp -I"E:\OOP Lab\Project\Flappy-Bird\FIrst_SFML_Project-master\SFML-2.6.1\include" -DSFML_STATIC
link:
	g++ main.o Game.o Bird.o Pipe.o -o main -L"E:\OOP Lab\Project\Flappy-Bird\FIrst_SFML_Project-master\SFML-2.6.1\lib" -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
clean:
	del -f *.o main game
