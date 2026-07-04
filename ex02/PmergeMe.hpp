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
        void createDequePairs(std::deque<int> &input, std::deque<std::pair<int, int> > &pairs, 
                        bool &hasLeftover, int &leftover);

        std::vector<int> fordJohnsonVector(std::vector<int> &input);
        std::deque<int> fordJohnsonDeque(std::deque<int> &input);

        void extractLargerElements(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &larger);
        void extractLargerElementsDeque(const std::deque<std::pair<int, int> > &pairs,
                        std::deque<int> &larger);   

        void reorderPairs(std::vector<std::pair<int, int> > &pairs,
                        const std::vector<int> &sortedLarger);
        void reorderPairsDeque(std::deque<std::pair<int, int> > &pairs,
                        const std::deque<int> &sortedLarger);

        void buildChains(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &mainChain, std::vector<int> &pend);
        void buildChainsDeque(const std::deque<std::pair<int, int> > &pairs,
                        std::deque<int> &mainChain, std::deque<int> &pend);

        void binaryInsertVector(std::vector<int> &chain, int value);
        size_t binaryInsertBoundedVector(std::vector<int> &chain, int value, size_t right);
        
        void binaryInsertDeque(std::deque<int> &chain, int value);
        size_t binaryInsertBoundedDeque(std::deque<int> &chain, int value, size_t right);

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
        void displayAfterDeque();
        void displayBeforeDeque();

        void process();
};

#endif