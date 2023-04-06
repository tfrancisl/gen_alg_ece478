#Genetic Algo Makefile TFL
TARGET = main
OBJS = main.o entity.o gen_alg_game.o bitset_genetics.o
CFLAGS = -Wall -I.



$(TARGET): $(OBJS)
	g++  $(CFLAGS) $(OBJS) -o $(TARGET)

gen_alg_game.o: gen_alg.h

clean:
	rm -f $(OBJS) $(TARGET)