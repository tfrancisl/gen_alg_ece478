#Genetic Algo Makefile TFL
TARGET = main
SRCS = main.cpp chromosome.cpp entity.cpp gen_alg_game.cpp bitset_genetics.cpp
OBJS = main.o chromosome.o entity.o gen_alg_game.o bitset_genetics.o
CFLAGS = -Wall -I.



$(TARGET): $(OBJS)
	g++  $(CFLAGS) $(OBJS) -o $(TARGET)

gen_alg_game.o: gen_alg.h

clean:
	rm -f $(OBJS) $(TARGET)