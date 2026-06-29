#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>
#include <utility>

class PmergeMe{
    private:
        std::vector<int> _vec;
        std::deque<int> _deq;

        void createPairs(std::vector<int> &input, std::vector<std::pair<int, int> > &pairs, 
                        bool &hasLeftover, int &leftover);
        void buildChains(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &mainChain, std::vector<int> &pend);

        void sortPairs(std::vector<std::pair<int, int> > &pairs);

        void fordJohnsonVector();
        void fordJohnsonDeque();

        void binaryInsertVector(std::vector<int> &chain, int value);
        void binaryInsertDeque(std::deque<int> &chain, int value);

        std::vector<size_t> jacobsthalOrder(size_t n);
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