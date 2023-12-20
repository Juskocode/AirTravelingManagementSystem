//
// Created by Utilizador on 03/12/2023.
//

#ifndef AIRBUSMANAGEMENTSYSTEM_PARSER_H
#define AIRBUSMANAGEMENTSYSTEM_PARSER_H

#include <unordered_map>
#include <map>
#include <fstream>
#include <istream>
#include <sstream>
#include "airport.h"
#include "airline.h"
#include "../network/graph.h"
using namespace std;


class Parser {
public:
    Parser();
    Airport::AirportH const& getAirports() const;
    Airline::AirlineH const& getAirlines() const;
    Airport::CityH const &getCity() const;
    Graph getGraph() const;
    unordered_map<string,int> getMap() const;

    /**
     * Transforms the map into a multimap and swaps it information\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n*log(m))</b>,n -> size of map src, insertion of map src key in multimap
     * </pre>
     * @tparam A - key
     * @tparam B - value
     * @param src
     * @return flipped multimap were all the keys turn to values and values to keys
    */
    static multimap<int,string> convertMap(const map<string,int>& m);

    Airport::AirportH airports;
    Airline::AirlineH airlines;
    Graph graph = Graph(0);

    unordered_map<string, int> idAirports;
    unordered_set<string> countries;
    unordered_set<string> cities;

    Airport::CityH airportsPerCity;
    unordered_map<string, list<string>> citiesPerCountry;

private:

    /**
     * Reads airports.csv file and stores the airports information in airportsPerCity, idAirports, airports, cities, countries,
     * citiesPerCountry and in the graphs function we store the airports according to an index\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n)</b>, n is the number of file lines
     * </pre>
     */
    void createAirports();

    /**
     * Reads airlines.csv file and stores the information in airlines\n\n
     *  * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n)</b>, n is the number of file lines
     * </pre>
     */
    void createAirlines();

    /**
     * Reads flights.csv file and stores the airports information in graphs about the flights
     * (airport of departure/arrival and distance between them)\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n*log(n))</b>, n is the numbers of file lines
     * </pre>
     */
    void createGraph();

};


#endif //AIRBUSMANAGEMENTSYSTEM_PARSER_H
