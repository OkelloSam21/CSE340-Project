CXX = g++
CXXFLAGS = -Wall -std=c++11

SOURCES = main.cpp lexer.cpp parser.cpp symbolTable.cpp
HEADERS = lexer.h parser.h symbolTable.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = parser

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)