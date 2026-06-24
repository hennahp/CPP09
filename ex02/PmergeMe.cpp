#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    *this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if(this != &other)
    {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::displayBefore()
{
    std::cout << "Before: ";
    for(size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;
}

void PmergeMe::displayAfter()
{
    std::cout << "After: ";
    for(size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;
}

void PmergeMe::parseInput(char **av)
{
    for(int i = 1; av[i]; i++)
    {
        std::string s = av[i];
        if(s.empty())
        {
            std::cerr << "Error" << std::endl;
            exit(1);
        }

        for(size_t j = 0; j < s.length(); j++)
        {
            if(!std::isdigit(s[j]))
            {
                std::cerr << "Error" << std::endl;
                exit(1);
            }
        }

        errno = 0;

        char *end;
        long value = std::strtol(s.c_str(), &end, 10);

        if(*end != '\0' || errno == ERANGE || value < 0 || value > INT_MAX)
        {
            std::cerr << "Error" << std::endl;
            exit(1);
        }

        int num = static_cast<int>(value);

        _vec.push_back(num);
        _deq.push_back(num);
    }
}

void PmergeMe::process()
{
    displayBefore();

    struct timeval start, end;

    gettimeofday(&start, NULL);
    sortVector();
    gettimeofday(&end, NULL);
    long vectTime = (end.tv_sec - start.tv_sec) * 10000000L + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    sortDeque();
    gettimeofday(&end, NULL);
    long deqTime = (end.tv_sec - start.tv_sec) * 10000000L + (end.tv_usec - start.tv_usec);

    displayAfter();

    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << vectTime << " us" << std::endl;

    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque : "
              << deqTime << " us" << std::endl;
}

