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

CFLAGS = -Wall -I$(IDIR)/ -std=c11  #c++11 is for compatibility with PSU servers
LDFLAGS = -pthread
EXE = header

.PHONY: all compile clean test 

#debug variable prints
#$(info Headers: $(HEADERS))
#$(info OBJ: $(OBJ))
#$(info SRC: $(SRC))
#$(info LDFLAGS: $(LDFLAGS))
#$(info CFLAGS: $(CFLAGS))



all : seeker seeker-base clean test

seeker : seeker.o
	@$(GXX) $(LDFLAGS) -o $@ $^

seeker.o : $(SRC_DIR)/seeker.c $(HEADERS)
	@$(GXX) $(CFLAGS) -c $< -o $@

seeker-base : seeker-base.o
	@$(GXX) $(LDFLAGS) -o $@ $^

seeker-base.o : $(SRC_DIR)/seeker-base.c $(HEADERS)
	@$(GXX) $(CFLAGS) -c $< -o $@

clean:
	@rm *.o

test: 
	@echo "compiled"
	@echo "running code "
	@echo "Running: timer"
	@echo "--------------------"
	@echo " "
	@echo "--------------------"
	@echo "Running: seeker-base"
	@echo "--------------------"
	@echo ""
	@echo ""

help:
	@echo "all compile clean seeker seeker-base help"
