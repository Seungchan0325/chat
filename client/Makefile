CC = g++

CFLAGS = -Wall -DDEBUG
LDFLAGS = -lws2_32

TARGET = chat_client
OBJECTS = main.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


%.o : %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm *.o chat_client