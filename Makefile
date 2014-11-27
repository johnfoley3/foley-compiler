## author: John Foley
## date: 9.25.14

# some flags that might need to be set
FLAGS = -c -Wall -pedantic -Wno-delete-non-virtual-dtor

# one command to rule them all
all : truc.cc parser.o scanner.o buffer.o token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o numtoken.o eoftoken.o
	g++ $(FLAGS) -o main truc.cc

# clean out all object files
clean : 
	rm -rf *.o *.h.gch

token.o : token.cc token.h
	g++ $(FLAGS) token.cc

keywordtoken.o : keywordtoken.h keywordtoken.cc token.h
	g++ $(FLAGS) keywordtoken.cc

punctoken.o : punctoken.h punctoken.cc token.h
	g++ $(FLAGS) punctoken.cc

reloptoken.o : reloptoken.h reloptoken.cc token.h
	g++ $(FLAGS) reloptoken.cc

addoptoken.o : addoptoken.h addoptoken.cc token.h
	g++ $(FLAGS) addoptoken.cc

muloptoken.o : muloptoken.h muloptoken.cc token.h
	g++ $(FLAGS) muloptoken.cc

idtoken.o : idtoken.h idtoken.cc token.h
	g++ $(FLAGS) idtoken.cc

numtoken.o : numtoken.h numtoken.cc token.h
	g++ $(FLAGS) numtoken.cc

eoftoken.o : eoftoken.h eoftoken.cc token.h
	g++ $(FLAGS) eoftoken.cc

buffer.o : buffer.h buffer.cc
	g++ $(FLAGS) buffer.cc

scanner.o : scanner.h scanner.cc token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o\
		numtoken.o eoftoken.o buffer.o
	g++ $(FLAGS) scanner.cc

parser.o : parser.h parser.cc 
	g++ $(FLAGS) parser.cc

truc.o : truc.cc
	g++ $(FLAGS) truc.cc
	