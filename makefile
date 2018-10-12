snake-master: main.o snake.o motion.o food.o score.o debug.o
	gcc -o snake-master main.o snake.o motion.o food.o score.o debug.o `pkg-config --libs allegro-5 allegro_dialog-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5`
	
main.o: main.c main.h snake.h motion.h food.h score.h debug.h color_names.h
	gcc -c main.c -o main.o
	
snake.o: snake.c snake.h main.h motion.h color_names.h
	gcc -c snake.c -o snake.o
	
motion.o: motion.c motion.h snake.h
	gcc -c motion.c -o motion.o
	
food.o: food.c food.h main.h snake.h score.h color_names.h
	gcc -c food.c -o food.o
	
score.o: score.c score.h food.h color_names.h
	gcc -c score.c -o score.o
	
debug.o: debug.c debug.h snake.h motion.h
	gcc -c debug.c -o debug.o
