//
// Created by freitas on 08-12-2023.
//

#include "Utils.h"

#include <utility>

Utils::Utils(Parser parser) : parser(std::move(parser)){}


bool Utils::isCountry(const string& country){
    auto i = parser.countries.find(country);
    if (i == parser.countries.end()) return false;
    return true;
}

bool Utils::isAirport(const Airport& airport){
    auto i = parser.airports.find(airport);
    if (i == parser.airports.end()) return false;
    return true;
}

bool Utils::isAirline(const Airline& airline) {
    auto i = parser.airlines.find(airline);
    if (i == parser.airlines.end()) return false;
    return true;
}

bool Utils::isCity(const string& city) {
    auto i = parser.cities.find(city);
    if (i == parser.cities.end()) return false;
    return true;
}

bool Utils::isValidCity(const string& country, const string& city) {
    for (const auto& c : parser.citiesPerCountry[country])
        if (city == c) return true;
    return false;
}
