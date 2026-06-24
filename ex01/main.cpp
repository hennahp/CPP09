#include "RPN.hpp"

int main(int ac, char **av)
{
    if(ac != 2)
    {
        std::cerr << "Usage: " << av[0] << " <RPN expression>" << std::endl;
        return 1;
    }

    RPN rpn;

    if(!rpn.evaluate(av[1]))
    {
        std::cerr << "Error: Invalid RPN expression." << std::endl;
        return 1;
    }

    std::cout << rpn.getResult() << std::endl;
    return 0;
}
