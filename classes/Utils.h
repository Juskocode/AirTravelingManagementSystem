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

    /**
     * Verifies if a country is in the countries unordered_set\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(1)</b>
     * </pre>
     * @param country - wanted country
     * @return true if country exists in countries, else false
     */
    bool isCountry(const string& country);

    /**
     * Verifies if a airport is in the airports unordered_set\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(1)</b>
     * </pre>
     * @param airport - wanted airport
     * @return true if airport exists in airports, else false
     */
    bool isAirport(const Airport& airport);

    /**
     * Verifies if a airline is in the airline unordered_set\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(1)</b>
     * </pre>
     * @param airline - wanted airline
     * @return true if airline exists in airlines, else false
     */
    bool isAirline(const Airline& airline);

    /**
     * Verifies if a city is in cities unordered_set\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(1)</b>
     * </pre>
     * @param city -city wanted to verify
     * @return true if city exists in cities, else false
     */
    bool isCity(const string& city);

    /**
     * Verifies if a country and city is in citiesPerCountry unordered_map\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n)</b>, n -> number of cities from the specific country
     * </pre>
     * @param country - wanted country
     * @param city - wanted city
     * @return true if country, city exists, else false
     */
    bool isValidCity(const string& country, const string& city);


};


#endif //AIRBUSMANAGEMENTSYSTEM_UTILS_H
