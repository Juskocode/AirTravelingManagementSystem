//
// Created by freitas on 08-12-2023.
//

#include "Utils.h"

#include <utility>

Utils::Utils(Parser parser) : parser(std::move(parser)){}

/**
 * Verifies if a country is in the countries unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param country - wanted country
 * @return true if country exists in countries, else false
 */
bool Utils::isCountry(const string& country){
    auto i = parser.countries.find(country);
    if (i == parser.countries.end()) return false;
    return true;
}
/**
 * Verifies if a airport is in the airports unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param airport - wanted airport
 * @return true if airport exists in airports, else false
 */
bool Utils::isAirport(const Airport& airport){
    auto i = parser.airports.find(airport);
    if (i == parser.airports.end()) return false;
    return true;
}
/**
 * Verifies if a airline is in the airline unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param airline - wanted airline
 * @return true if airline exists in airlines, else false
 */
bool Utils::isAirline(const Airline& airline) {
    auto i = parser.airlines.find(airline);
    if (i == parser.airlines.end()) return false;
    return true;
}
/**
 * Verifies if a city is in cities unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param city -city wanted to verify
 * @return true if city exists in cities, else false
 */
bool Utils::isCity(const string& city) {
    auto i = parser.cities.find(city);
    if (i == parser.cities.end()) return false;
    return true;
}
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
bool Utils::isValidCity(const string& country, const string& city) {
    for (const auto& c : parser.citiesPerCountry[country])
        if (city == c) return true;
    return false;
}
