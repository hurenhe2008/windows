TARGET_BIN = ./bin

vpath %.cpp ./
vpath %.cpp ./utils

#wildcard 扩展通配符
#notdir 去除路径
#patsubst 替换通配符
INCS = -I. -I./utils
SRCS = $(wildcard *.cpp ./utils/*.cpp)
DIRS = $(notdir ${SRCS})
OBJS = $(patsubst %.cpp, %.o, ${DIRS})
LIBS = -lpthread

CC = gcc
CXX = g++
CFLAGS = -c -g -Wall -Wreorder -std=c++11 

APP = unit_test
BIN_APP = $(TARGET_BIN)/$(APP)

all: $(BIN_APP)

$(BIN_APP): $(OBJS) $(LIBS)
	$(CC) $^ -o $@

.cpp.o:
	$(CXX) -c $(CFLAGS) $(INCS) $< -o $@


clean:
	rm -rf *.o $(BIN_APP) *.~
