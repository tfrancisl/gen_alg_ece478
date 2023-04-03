#Genetic Algo Makefile TFL
TARGET = gen
OBJS = gen.o chromosome.o entity.o
CFLAGS = -Wall -I.
LIBS =

$(TARGET): $(OBJS)
	g++ -o $(TARGET) $(CFLAGS) -g $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)