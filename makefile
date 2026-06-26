
CXX = g++

CXXFLAGS = -std=c++11 -Wall 

TARGET = biblioteca


SRCS = main.cpp \
       Pessoa.cpp \
       Livro.cpp \
       Usuario.cpp \
       Bibliotecario.cpp \
       Emprestimo.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
