CC=g++
FLAGS= -O3 -Wall
LFLAGS= -lm

%.o : %.cpp
	$(CC) -c $(CFLAGS) -DTIME=$(t) $< -o $@

FILES = main.o \
        particleBox.o

main: $(FILES)
	$(CC) $(FILES) -o time$(t).exe $(LFLAGS)
	rm -f *.o

plots: plotTalk.o
	$(CC) plotTalk.o -o plots.exe $(LFLAGS)
	rm -f *.o

clean:
	-rm *.exe
	-rm *.dat
	
