DEFAULT_TARGET=smexample
OBJECTS = SMSampleCode.o evtLib/myEvtLib.o

CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc


# Generate application
$(DEFAULT_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(DEFAULT_TARGET) $(OBJECTS)

# Generate object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

.phony: all
.phony: clean
.phony:	run

all: clean run	

clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)
