#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>

class PmergeMe{
    private:
        std::vector<int> _vec;
        std::deque<int> _deq;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void parseInput(char **av);

        void sortVector();
        void sortDeque();

        void displayBefore();
        void displayAfter();

        void process();
};

#endif