#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
    _database = other._database;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if(this != &other)
    {
        _database = other._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    std::string line;
    if(!std::getline(file, line))
    {
        std::cerr << "Error: Could not read from file." << std::endl;
        return;
    }
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;

        std::stringstream ss(line);
        std::string date;
        std::string rateStr;

        if(!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
        {
            std::cerr << "Error: Invalid line format in database." << std::endl;
            continue;
        }

        // if(!date.empty() && date[date.size() - 1] == ' ')
        //     date.erase(date.size() - 1);

        while(!date.empty() && (date[date.size() - 1] == ' ' || date[date.size() - 1] == '\t'))
            date.erase(date.size() - 1);

        std::stringstream rateStream(rateStr);
        double rate;

        if(!(rateStream >> rate))
        {
            std::cerr << "Error: Invalid rate value in database." << std::endl;
            continue;
        }

        _database[date] = rate;
    }
}

double BitcoinExchange::findRate(const std::string &date) const
{
    std::map<std::string, double>::const_iterator it;
    it = _database.lower_bound(date);

    if(it == _database.begin() && it->first != date)
    {
        std::cerr << "Error: No valid earlier date found for " << date << "." << std::endl;
        return -1;
    }

    if(it == _database.end() || it->first != date)
    {
        --it;
    }

    return it->second;
}

