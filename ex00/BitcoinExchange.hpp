#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

class BitcoinExchange {
    private:
        std::map<std::string, double>_database;

        bool isValidDate(const std::string &date) const;
        bool isValidValue(const std::string &value) const;

        double findRate(const std::string &date) const;
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();

        void loadDatabase(const std::string &filename);
        void processInputFile(const std::string &filename) const;
};




#endif