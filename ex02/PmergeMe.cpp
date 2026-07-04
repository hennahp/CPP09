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
    std::cout << "Deque Before: ";
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
    std::cout << "Deque After: ";
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
    if(!av[1])
    {
        std::cerr << "Error" << std::endl;
        exit(1);
    }   
    
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
    // displayBeforeDeque();

    struct timeval start, end;

    gettimeofday(&start, NULL);
    sortVector();
    gettimeofday(&end, NULL);
    long vectTime = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    sortDeque();
    gettimeofday(&end, NULL);
    long deqTime = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);

    displayAfter();
    // displayAfterDeque();

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

void PmergeMe::extractLargerElements(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &larger)
{
    for(size_t i = 0; i < pairs.size(); i++)
    {
        larger.push_back(pairs[i].second);
    }
}

void PmergeMe::reorderPairs(std::vector<std::pair<int, int> > &pairs,
                        const std::vector<int> &sortedLarger)
{
    std::vector<std::pair<int, int> > ordered;

    for(size_t i = 0; i < sortedLarger.size(); i++)
    {
        for(size_t j = 0; j < pairs.size(); j++)
        {
            if(pairs[j].second == sortedLarger[i])
            {
                ordered.push_back(pairs[j]);
                pairs.erase(pairs.begin() + j);
                break;
            }
        }
    }
    pairs = ordered;
}

std::vector<int> PmergeMe::fordJohnsonVector(std::vector<int> &input)
{
    if(input.size() <= 1)
        return input;
    
    std::vector<std::pair<int, int> > pairs;
    bool hasLeftover = false;
    int leftover = -1;

    createPairs(input, pairs, hasLeftover, leftover);

    std::vector<int> larger;
    extractLargerElements(pairs, larger);
    larger = fordJohnsonVector(larger);
    reorderPairs(pairs, larger);

    std::vector<int> mainChain;
    std::vector<int> pend;
    buildChains(pairs, mainChain, pend);

    std::vector<size_t> bound(pend.size());
    for(size_t i = 0; i < pend.size(); i++)
        bound[i] = i;

    std::vector<size_t> order = jacobsthalOrder(pend.size());
    for(size_t k = 0; k < order.size(); k++)
    {
        size_t idx = order[k];
        size_t insertPos = binaryInsertBoundedVector(mainChain, pend[idx], bound[idx]);
        for(size_t j = 0; j < bound.size(); j++)
        {
            if(bound[j] >= insertPos)
                bound[j] += 1;
        }
    }    

    if(hasLeftover)
        binaryInsertVector(mainChain, leftover);

    return mainChain;
}

void PmergeMe::buildChains(const std::vector<std::pair<int, int> > &pairs,
                        std::vector<int> &mainChain, std::vector<int> &pend)
{
    mainChain.clear();
    pend.clear();
    for(size_t i = 0; i < pairs.size(); i++)
    {
        mainChain.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }
}

size_t PmergeMe::binaryInsertBoundedVector(std::vector<int> &chain, int value, size_t right)
{
    size_t left = 0;

    while(left < right)
    {
        size_t mid = left + (right - left) / 2;

        if(chain[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }

    chain.insert(chain.begin() + left, value);
    return left;
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
    _vec = fordJohnsonVector(_vec);
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

void PmergeMe::extractLargerElementsDeque(const std::deque<std::pair<int, int> > &pairs,
                        std::deque<int> &larger)
{
    for(size_t i = 0; i < pairs.size(); i++)
    {
        larger.push_back(pairs[i].second);
    }
}

void PmergeMe::reorderPairsDeque(std::deque<std::pair<int, int> > &pairs,
                        const std::deque<int> &sortedLarger)
{
    std::deque<std::pair<int, int> > ordered;

    for(size_t i = 0; i < sortedLarger.size(); i++)
    {
        for(size_t j = 0; j < pairs.size(); j++)
        {
            if(pairs[j].second == sortedLarger[i])
            {
                ordered.push_back(pairs[j]);
                pairs.erase(pairs.begin() + j);
                break;
            }
        }
    }
    pairs = ordered;
}

void PmergeMe::buildChainsDeque(const std::deque<std::pair<int, int> > &pairs,
                        std::deque<int> &mainChain, std::deque<int> &pend)
{
    mainChain.clear();
    pend.clear();
    for(size_t i = 0; i < pairs.size(); i++)
    {
        mainChain.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
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

size_t PmergeMe::binaryInsertBoundedDeque(std::deque<int> &chain, int value, size_t right)
{
    size_t left = 0;

    while(left < right)
    {
        size_t mid = left + (right - left) / 2;

        if(chain[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }

    chain.insert(chain.begin() + left, value);
    return left;
}

std::deque<int> PmergeMe::fordJohnsonDeque(std::deque<int> &input)
{
    if(input.size() <= 1)
        return input;

    std::deque<std::pair<int, int> > pairs;
    bool hasLeftover = false;
    int leftover = -1;

    createDequePairs(input, pairs, hasLeftover, leftover);

    std::deque<int> larger;
    extractLargerElementsDeque(pairs, larger);
    larger = fordJohnsonDeque(larger);
    reorderPairsDeque(pairs, larger);

    std::deque<int> mainChain;
    std::deque<int> pend;
    buildChainsDeque(pairs, mainChain, pend);

    std::vector<size_t> bound(pend.size());
    for(size_t i = 0; i < pend.size(); i++)
        bound[i] = i;

    std::vector<size_t> order = jacobsthalOrder(pend.size());
    for(size_t k = 0; k < order.size(); k++)
    {
        size_t idx = order[k];
        size_t insertPos = binaryInsertBoundedDeque(mainChain, pend[idx], bound[idx]);
        for(size_t j = 0; j < bound.size(); j++)
        {
            if(bound[j] >= insertPos)
                bound[j] += 1;      
        }
    }
    if(hasLeftover)
        binaryInsertDeque(mainChain, leftover);
    return mainChain;
}

void PmergeMe::sortDeque()
{
    _deq = fordJohnsonDeque(_deq);
}