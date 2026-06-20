#include "BitcoinExchange.hpp"

static void ltrim(std::string &str)
{
    while(!str.empty() && (str[0] == ' ' || str[0] == '\t'))
        str.erase(0, 1);
}

static void rtrim(std::string &str)
{
    while(!str.empty() && (str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t'))
        str.erase(str.size() - 1);
}

static void trim(std::string &str)
{
    ltrim(str);
    rtrim(str);
}

static bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

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

bool BitcoinExchange::findRate(const std::string &date, double &rate) const
{
	if (_database.empty())
		return false;

	std::map<std::string, double>::const_iterator it = _database.lower_bound(date);

	// exact match
	if (it != _database.end() && it->first == date)
	{
		rate = it->second;
		return true;
	}

	// too early (no smaller date exists)
	if (it == _database.begin())
		return false;

	// fallback to previous date
	if (it == _database.end())
		--it;
	else
		--it;

	rate = it->second;
	return true;
}

bool BitcoinExchange::isValidDate(const std::string &date)const
{
    if(date.length() != 10 ||
       date[4] != '-' || date[7] != '-')
        return false;

    for(size_t i = 0; i < date.length(); i++)
    {
        if(i == 4 || i == 7)
            continue;
        if(!std::isdigit(date[i]))
            return false;
    }
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if(month < 1 || month > 12)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                         31, 31, 30, 31, 30, 31};

    if(month ==2)
    {
        if(isLeapYear(year))
        {
            if(day < 1 || day > 29)
                return false;
        }
        else
        {
            if(day < 1 || day > 28)
                return false;
        }
    }
    else
    {
        if(day < 1 || day > daysInMonth[month - 1])
            return false;
    }
    return true;
}

void BitcoinExchange::processInputFile(const std::string &filename) const
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }

    std::string line;
    
    // Skip the first line (header)
    if(!std::getline(file, line))
    {
        std::cerr << "Error: Could not read from input file." << std::endl;
        return;
    }

    while(std::getline(file, line))
    {
        if(line.empty())
            continue;

        std::string date;
        std::string valueStr;

        //split the line into date and value with '|' as the delimiter
        size_t pos = line.find('|');
        if(pos == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        date = line.substr(0, pos);
        valueStr = line.substr(pos + 1);

        //trim whitespace from date and valueStr
        trim(date);
        trim(valueStr);

        //validate date and value
        std::stringstream ss(valueStr);
        double value;
        char c;

        if(!(ss >> value) || (ss >> c))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if(value < 0)
        {
            std::cerr << "Error: Not a positive number." << std::endl;
            continue;
        }
        if(value > 1000)
        {
            std::cerr << "Error: Too large a number." << std::endl;
            continue;
        }
        if(!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        double rate;
        if(!findRate(date, rate))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }    
        
        double result = value * rate;

        std::cout << date << " => " << value << " = "  << result << std::endl;
    }
}
