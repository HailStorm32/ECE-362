GXX = gcc

#Path to .h files
IDIR = include

#Path to .c files
SRC_DIR = src

#Path to .o files
OBJ_DIR = $(SRC_DIR)

#All .c files
SRC = $(wildcard $(SRC_DIR)/*.c)

#All .o files
OBJ := $(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c))

#All .h files
HEADERS = $(wildcard $(IDIR)/*.h)

CFLAGS = -Wall -I$(IDIR)/ -std=c11 #c++11 is for compatibility with PSU servers
LDFLAGS = 
EXE = header

.PHONY: all compile clean test 

#debug variable prints
#$(info Headers: $(HEADERS))
#$(info OBJ: $(OBJ))
#$(info SRC: $(SRC))
#$(info LDFLAGS: $(LDFLAGS))
#$(info CFLAGS: $(CFLAGS))



all : timer piper clean test

timer : timer.o
	@$(GXX) -o $@ $^

timer.o : $(SRC_DIR)/timer.c $(HEADERS)
	@$(GXX) $(CFLAGS) -c $< -o $@

piper : piper.o
	@$(GXX) -o $@ $^

piper.o : $(SRC_DIR)/piper.c $(HEADERS)
	@$(GXX) $(CFLAGS) -c $< -o $@

clean:
	@rm *.o

test: 
	@echo "compiled"
	@echo "running code "
	@echo "--------------------"
	./$(EXE) test.txt -5
	@echo ""
	./$(EXE) test.txt
	@echo ""
	./$(EXE) test.txt -15
	@echo ""
	./$(EXE) test.txt -2
	@echo ""
	./$(EXE) -4

help:
	@echo "all compile clean test help"
