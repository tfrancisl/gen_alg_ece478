#Genetic Algo Makefile TFL
TARGET = main
OBJS = main.o chromosome.o entity.o gen_alg_game.o bitset_genetics.o
CFLAGS = -Wall -I.
LIBS =

$(TARGET): $(OBJS)
	g++ -o $(TARGET) $(CFLAGS) -g $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)