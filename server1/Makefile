# Compiler and flags
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17

# Project files
NAME := webserv
SRCS := main.cpp server.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := server.hpp

# Rules
all: clean_port $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean_port:
	./clean.sh

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re clean_port
