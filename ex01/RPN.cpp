#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &other)
{
    *this = other;
}

RPN &RPN::operator=(const RPN &other)
{
    if(this != &other)
    {
        _stack = other._stack;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::isNumber(const std::string &token) const
{
    if (token.empty())
        return false;

    size_t i = 0;

    if (token[i] == '+' || token[i] == '-')
    {
        if (token.size() == 1)
            return false;
        i++;
    }

    for (; i < token.size(); i++)
    {
        if (!isdigit(token[i]))
            return false;
    }

    return true;
}

bool RPN::isOperator(const std::string &token) const
{
    return token == "+" || token == "-" || token == "*" || token == "/";
}

bool RPN::applyOperator(const std::string &op)
{
    if(_stack.size() < 2)
        return false;

    int b = _stack.top();
    _stack.pop();
    int a = _stack.top();
    _stack.pop();

    if(op == "+")
        _stack.push(a + b);
    else if(op == "-")
        _stack.push(a - b);
    else if(op == "*")
        _stack.push(a * b);
    else if(op == "/")
    {
        if(b == 0)
            return false;
        _stack.push(a / b);
    }
    return true;
}

bool RPN::evaluate(const std::string &expression)
{
    std::istringstream iss(expression);
    std::string token;

    while(iss >> token)
    {
        if(isNumber(token))
        {
            int _value = std::atoi(token.c_str());
            _stack.push(_value);
        }
        else if(isOperator(token))
        {
            if(!applyOperator(token))
                return false;
        }
        else
            return false;
    }
    if(_stack.size() != 1)
        return false;
    return true;
}

int RPN::getResult() const
{
    if(_stack.empty())
        throw std::runtime_error("Stack is empty. No result available.");
    return _stack.top();
}