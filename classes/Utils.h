//
// Created by freitas on 08-12-2023.
//

#ifndef AIRBUSMANAGEMENTSYSTEM_UTILS_H
#define AIRBUSMANAGEMENTSYSTEM_UTILS_H

#include "Parser.h"

class Utils {
private:
    Parser parser;

public:

    Utils(Parser parser);

    bool isCountry(const string& country);
    bool isAirport(const Airport& airport);
    bool isAirline(const Airline& airline);
    bool isCity(const string& city);
    bool isValidCity(const string& country, const string& city);


};


#endif //AIRBUSMANAGEMENTSYSTEM_UTILS_H
