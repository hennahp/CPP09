#include "PmergeMe.hpp"

int main(int ac, char **av)
{
    if(ac < 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    PmergeMe pm;

    pm.parseInput(av);
    pm.process();

    return 0;
}