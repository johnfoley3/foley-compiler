## author: John Foley
## date: 9.25.14

# some flags that might need to be set
FLAGS = -c

# one command to rule them all
all : 

# clean out all object files
clean : 
	rm -rf *.o *.h.gch

token.o : token.cc token.h
	g++ $(FLAGS) token.cc token.h 

keywordtoken.o : keywordtoken.h keywordtoken.cc token.h
	g++ $(FLAGS) keywordtoken.h keywordtoken.cc token.h

punctoken.o : punctoken.h punctoken.cc token.o
	g++ $(FLAGS) punctoken.h punctoken.cc token.o

reloptoken.o : reloptoken.h reloptoken.cc token.o
	g++ $(FLAGS) reloptoken.h reloptoken.cc token.o

addoptoken.o : addoptoken.h addoptoken.cc token.o
	g++ $(FLAGS) addoptoken.h addoptoken.cc token.o

muloptoken.o : muloptoken.h muloptoken.cc token.o
	g++ $(FLAGS) muloptoken.h muloptoken.cc token.o

idtoken.o : idtoken.h idtoken.cc token.o
	g++ $(FLAGS) idtoken.h idtoken.h token.o

numtoken.o : numtoken.h numtoken.cc token.o
	g++ $(FLAGS) numtoken.h numtoken.h token.o

eoftoken.o : eoftoken.h eoftoken.cc token.o
	g++ $(FLAGS) eoftoken.h eoftoken.h token.o

buffer.o : buffer.h buffer.cc
	g++ $(FLAGS) buffer.h buffer.cc

scanner.o : scanner.h scanner.cc token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o\
		numtoken.o eoftoken.o buffer.o
	g++ $(FLAGS) scanner.h scanner.cc token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o\
		numtoken.o eoftoken.o buffer.o 
