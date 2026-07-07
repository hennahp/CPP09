#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <list>
#include <string>
#include <sstream>
#include <cstdlib>

class RPN
{
    private:
        std::stack<int, std::list<int> >_stack;

        bool applyOperator(const std::string &op);
        bool isOperator(const std::string &token) const;
        bool isNumber(const std::string &token) const;
    public:
        RPN();
        RPN(const RPN &other);
        RPN &operator=(const RPN &other);
        ~RPN();

        bool evaluate(const std::string &expression);
        int getResult() const;
};

#endif