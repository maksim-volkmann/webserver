# Compiler and flags
CXX      = gcc
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Target executable
NAME     = webserver

# Source files
SRCS     = basic_webserver.cpp

# Object files (generated from source files)
OBJS     = $(SRCS:.cpp=.o)

# Rule to build the executable
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) -lstdc++

# Rule to clean up object files
clean:
	rm -f $(OBJS)

# Rule to clean up everything (object files + executable)
fclean: clean
	rm -f $(NAME)

# Rule to rebuild everything from scratch
re: fclean all
