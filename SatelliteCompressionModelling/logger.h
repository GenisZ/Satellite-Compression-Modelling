#ifndef Logger_HPP
#define Logger_HPP

#include <fstream>
#include <iostream>

class Logger {

public:
    // ctor
    explicit Logger(const char* fname = "log.txt")
    {
        myFile.open(fname);

        if (myFile.is_open()) {
            myFile << "Log file created" << std::endl << std::endl;
        }
    }

    // dtor
    ~Logger() {
        if (myFile.is_open()) {
            myFile << std::endl << std::endl;

            myFile.close();
        }
    }

    friend Logger& operator << (Logger& logger, const char* text) {
        std::cout << text;
        logger.myFile << text;
        return logger;
    }

    friend Logger& operator << (Logger& logger, int text) {
        std::cout << text;
        logger.myFile << text;
        return logger;
    }

    friend Logger& operator << (Logger& logger, std::string text) {
        std::cout << text;
        logger.myFile << text;
        return logger;
    }


    // Make it Non Copyable
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    const char *endl = "\n";

private:

    std::ofstream           myFile;

}; // class end


#endif // Logger_HPP