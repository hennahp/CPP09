#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

class BitcoinExchange {
    private:
        std::map<std::string, double>_database;

        bool isValidDate(const std::string &date) const;

        bool findRate(const std::string &date, double &rate) const;
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();

        void loadDatabase(const std::string &filename);
        void processInputFile(const std::string &filename) const;
};




#endif