## author: John Foley
## date: 9.25.14

# some flags that might need to be set
FLAGS = -c

# one command to rule them all
all : test_scanner.cc scanner.o buffer.o
	g++ -o main test_scanner.cc scanner.o buffer.o token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o numtoken.o eoftoken.o

# clean out all object files
clean : 
	rm -rf *.o *.h.gch

token.o : token.cc token.h
	g++ $(FLAGS) token.cc token.h 

keywordtoken.o : keywordtoken.h keywordtoken.cc token.h
	g++ $(FLAGS) keywordtoken.h keywordtoken.cc token.h

punctoken.o : punctoken.h punctoken.cc token.h
	g++ $(FLAGS) punctoken.h punctoken.cc token.h

reloptoken.o : reloptoken.h reloptoken.cc token.h
	g++ $(FLAGS) reloptoken.h reloptoken.cc token.h

addoptoken.o : addoptoken.h addoptoken.cc token.h
	g++ $(FLAGS) addoptoken.h addoptoken.cc token.h

muloptoken.o : muloptoken.h muloptoken.cc token.h
	g++ $(FLAGS) muloptoken.h muloptoken.cc token.h

idtoken.o : idtoken.h idtoken.cc token.h
	g++ $(FLAGS) idtoken.h idtoken.cc token.h

numtoken.o : numtoken.h numtoken.cc token.h
	g++ $(FLAGS) numtoken.h numtoken.cc token.h

eoftoken.o : eoftoken.h eoftoken.cc token.h
	g++ $(FLAGS) eoftoken.h eoftoken.cc token.h

buffer.o : buffer.h buffer.cc
	g++ $(FLAGS) buffer.h buffer.cc

test_scanner : test_scanner.cc scanner.o
	g++ -o main test_scanner.cc buffer.o

scanner.o : scanner.h scanner.cc token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o\
		numtoken.o eoftoken.o buffer.o
	g++ $(FLAGS) scanner.h scanner.cc token.h keywordtoken.h punctoken.h\
		reloptoken.h addoptoken.h muloptoken.h idtoken.h\
		numtoken.h eoftoken.h buffer.h 

