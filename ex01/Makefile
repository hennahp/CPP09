RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

NAME = RPN

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp RPN.cpp
OBJS = $(SRCS:.cpp=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BLUE)Compilation complete. Executable $(NAME) created.$(RESET)"
	@echo ""
	@echo "$(GREEN)"
	@echo "======================================="
	@echo "             RPN"
	@echo "======================================="
	@echo "$(RESET)"
	@echo "                         RPN Project"
	@echo "$(RESET)"

%.o: %.cpp RPN.hpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled $<$(RESET)"
	
clean:
	@$(RM) $(OBJS)
	@echo "$(YELLOW)Cleaned up object files.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Cleaned up executable file.$(RESET)"

re: fclean all	

.PHONY: all clean fclean re