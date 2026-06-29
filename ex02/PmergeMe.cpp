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
    {
        if(i != 0)
            std::cout << " ";
        std::cout << _vec[i];
    }    
    std::cout << std::endl;
}

void PmergeMe::displayBeforeDeque()
{
    std::cout << "Before: ";
    for(size_t i = 0; i < _deq.size(); i++)
    {
        if(i != 0)
            std::cout << " ";
        std::cout << _deq[i];
    }    
    std::cout << std::endl;
}

void PmergeMe::displayAfter()
{
    std::cout << "After: ";
    for(size_t i = 0; i < _vec.size(); i++)
    {
        if(i != 0)
            std::cout << " ";
        std::cout << _vec[i];
    }
    std::cout << std::endl;
}

void PmergeMe::displayAfterDeque()
{
    std::cout << "After: ";
    for(size_t i = 0; i < _deq.size(); i++)
    {
        if(i != 0)
            std::cout << " ";
        std::cout << _deq[i];
    }
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
    // displayBefore();
    displayBeforeDeque();

    struct timeval start, end;

    gettimeofday(&start, NULL);
    sortVector();
    gettimeofday(&end, NULL);
    long vectTime = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    sortDeque();
    gettimeofday(&end, NULL);
    long deqTime = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);

    // displayAfter();
    displayAfterDeque();

    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << vectTime << " us" << std::endl;

    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque : "
              << deqTime << " us" << std::endl;
}

void PmergeMe::createPairs(std::vector<int> &input, std::vector<std::pair<int, int> > &pairs, 
                        bool &hasLeftover, int &leftover)
{
    hasLeftover = false;
    leftover = -1;
    
    size_t i = 0;
    while(i + 1 < input.size())
    {
        int first = input[i];
        int second = input[i + 1];
        if(first > second)
            std::swap(first, second);
        
        pairs.push_back(std::make_pair(first, second));
        i += 2;
    }

    if(i < input.size())
    {
        hasLeftover = true;
        leftover = input[i];
    }
}

void PmergeMe::sortPairs(std::vector<std::pair<int, int> > &pairs)
{
    for(size_t i = 0; i < pairs.size(); i++)
    {
        for(size_t j = i + 1; j < pairs.size(); j++)
        {
            if(pairs[i].second > pairs[j].second)
                std::swap(pairs[i], pairs[j]);
        }
    }
}

void PmergeMe::sortDequePairs(std::deque<std::pair<int, int> > &pairs)
{
    for(size_t i = 0; i < pairs.size(); i++)
    {
        for(size_t j = i + 1; j < pairs.size(); j++)
        {
            if(pairs[i].second > pairs[j].second)
                std::swap(pairs[i], pairs[j]);
        }
    }
}

void PmergeMe::buildChains(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &mainChain, std::vector<int> &pend)
{
    for(size_t i = 0; i < pairs.size(); i++)
    {
        mainChain.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }
}

void PmergeMe::binaryInsertVector(std::vector<int> &chain, int value)
{
    size_t left = 0;
    size_t right = chain.size();

    while(left < right)
    {
        size_t mid = left + (right - left) / 2;

        if(chain[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }

    chain.insert(chain.begin() + left, value);
}

std::vector<size_t> PmergeMe::jacobsthalOrder(size_t n)
{
    std::vector<size_t>order;
    std::vector<bool>used(n, false);

    if(n == 0)
        return order;

    std::vector<size_t> jacobsthal;
    jacobsthal.push_back(1);
    jacobsthal.push_back(3);

    while(jacobsthal.back() < n)
    {
        size_t s = jacobsthal.size();
        size_t next = jacobsthal[s - 1] + 2 * jacobsthal[s - 2];
        jacobsthal.push_back(next);
    }

    size_t prev = 0;
    for(size_t i = 0; i < jacobsthal.size(); i++)
    {
        size_t end = std::min(jacobsthal[i], n);
        for(size_t j = end; j > prev; j--)
        {
            if(!used[j - 1])
            {
                order.push_back(j - 1);
                used[j - 1] = true;
            }
        }
        prev = end;
    }
    for(size_t i = 0; i < n; i++)
    {
        if(!used[i])
            order.push_back(i);
    }
    return order;
}

void PmergeMe::sortVector()
{
    std::vector<std::pair<int, int> > pairs;
    std::vector<int> mainChain;
    std::vector<int> pend;

    bool hasLeftover = false;
    int leftover = -1;

    createPairs(_vec, pairs, hasLeftover, leftover);
    sortPairs(pairs);
    buildChains(pairs, mainChain, pend);

    std::vector<size_t> order = jacobsthalOrder(pend.size());

    for(size_t i = 0; i < order.size(); i++)
    {
         binaryInsertVector(mainChain, pend[order[i]]);
    }  
    if(hasLeftover)
        binaryInsertVector(mainChain, leftover);

    _vec = mainChain;
}

void PmergeMe::createDequePairs(std::deque<int> &input, std::deque<std::pair<int, int> > &pairs, 
                        bool &hasLeftover, int &leftover)
{
    hasLeftover = false;
    leftover = -1;
    
    size_t i = 0;
    while(i + 1 < input.size())
    {
        int first = input[i];
        int second = input[i + 1];
        if(first > second)
            std::swap(first, second);
        
        pairs.push_back(std::make_pair(first, second));
        i += 2;
    }

    if(i < input.size())
    {
        hasLeftover = true;
        leftover = input[i];
    }
}

void PmergeMe::binaryInsertDeque(std::deque<int> &chain, int value)
{
    size_t left = 0;
    size_t right = chain.size();

    while(left < right)
    {
        size_t mid = left + (right - left) / 2;

        if(chain[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }

    chain.insert(chain.begin() + left, value);
}

void PmergeMe::sortDeque()
{
    std::deque<std::pair<int, int> > pairs;
    std::deque<int> mainChain;
    std::deque<int> pend;

    bool hasLeftover = false;
    int leftover = -1;

    createDequePairs(_deq, pairs, hasLeftover, leftover);
    sortDequePairs(pairs);
    
    for(size_t i = 0; i < pairs.size(); i++)
    {
        mainChain.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }

    std::vector<size_t> order = jacobsthalOrder(pend.size());

    for(size_t i = 0; i < order.size(); i++)
    {
         binaryInsertDeque(mainChain, pend[order[i]]);
    }  
    if(hasLeftover)
        binaryInsertDeque(mainChain, leftover);

    _deq = mainChain;
}