## author: John Foley
## date: 9.25.14
## last edit: 11.27.14

# some flags that might need to be set
FLAGS = -c -Wall -pedantic -Wno-delete-non-virtual-dtor

# one command to rule them all
truc : truc.cc parser.o scanner.o buffer.o token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o numtoken.o eoftoken.o\
		symbol_table.o register.o register_allocator.o emitter.o
	g++ -Wall -pedantic -o main truc.cc parser.o scanner.o buffer.o token.o keywordtoken.o punctoken.o\
		reloptoken.o addoptoken.o muloptoken.o idtoken.o numtoken.o eoftoken.o\
		symbol_table.o register.o register_allocator.o emitter.o

# clean out all object files
clean : 
	rm -rf *.o *.h.gch main

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

symbol_table.o : symbol_table.h symbol_table.cc 
	g++ $(FLAGS) symbol_table.cc

register.o : register.h register.cc
	g++ $(FLAGS) register.cc

register_allocator.o : register_allocator.h register_allocator.cc
	g++ $(FLAGS) register_allocator.cc

emitter.o : emitter.h emitter.cc
	g++ $(FLAGS) emitter.cc
