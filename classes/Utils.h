//
// Created by freitas on 08-12-2023.
//

#ifndef AIRBUSMANAGEMENTSYSTEM_UTILS_H
#define AIRBUSMANAGEMENTSYSTEM_UTILS_H

#include "Parser.h"

class Utils : public Parser{

public:

    Utils();

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

    vector<string> localAirports(double, double, double) const;


    /**
     * Calculates the total number of flights\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|)</b>, V -> number of nodes
     * </pre>
     * @return number of flights
     */
    int nrFlights();

    /**
     * Calculates the airports that exist in a radius of a certain coordinate\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|*log(n))</b>, n -> number of airports that exist in a certain radius, V-> number of nodes
     * </pre>
     * @param latitude - latitude of the center of the circumference
     * @param longitude -longitude of the center of the circumference
     * @param radius - radius of the circumference
     * @return vector of codes of airports that exist in that range
     */
    list<pair<string,string>> processFlight(int&, const vector<string>&, const vector<string>&, const Airline::AirlineH&);

    /**
     * Calculates the smallest amount of flights possible to get to a specific airport from another airport\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n*m*(|V|+|E|))</b>,n -> size of src vector, m -> size of dest vector,V -> number of nodes, E -> number of edges
     * </pre>
     * @param src  - source node
     * @param dest - final node
     * @param airline - airlines available for use (if empty, use all airlines)
     * @return list of all best possible paths
     */
    list<pair<string,string>> processDistance(double&, const vector<string>&, const vector<string>&, const Airline::AirlineH&);

    /**
     * Calculates the number of airports that belong to each country\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n*log(m))</b>,n -> number of cities, m -> number of values in airportsPerCountry map
     * </pre>
     */
    void countAirportsPerCountry();

    /**
     * Calculates the number of airlines that are founded in each country\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n)</b>, n -> number of airlines
     * </pre>
     * @param country - country that the user wants to get information from
     * @return number of airlines
     */
    int countAirlinesPerCountry(const string& country);


};


#endif //AIRBUSMANAGEMENTSYSTEM_UTILS_H
