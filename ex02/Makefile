RED = \033[31m	
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RESET = \033[0m

NAME = PmergeMe

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp PmergeMe.cpp
OBJS = $(SRCS:.cpp=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BLUE)Compilation complete. Executable $(NAME) created.$(RESET)"
	@echo ""
	@echo "$(GREEN)"
	@echo "======================================="
	@echo "             PmergeMe"
	@echo "======================================="
	@echo "$(RESET)"
	@echo "                         PmergeMe Project"
	@echo "$(RESET)"

%.o: %.cpp PmergeMe.hpp
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