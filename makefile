SRCS = malloc.c
EXT  = .c
OBJS = $(SRCS:$(EXT)=.o)
EXEC = malloc.so
CXX  = gcc
CXXFLAGS = -Wall -fPIC -shared -D_GNU_SOURCE
LIBS = -ldl

all:$(EXEC)


$(EXEC):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJS)
