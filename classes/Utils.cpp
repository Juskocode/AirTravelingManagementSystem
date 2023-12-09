
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

int Utils::nrFlights(){
    int nrFlights = 0;
    for (const auto& node: parser.graph.getVertexSet())
        nrFlights += node->getAdj().size();
    return nrFlights;
}

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
vector<string> Utils::localAirports(double latitude, double longitude, double radius) const {
    vector<string> localAirports;
    double latitude1,longitude1;
    for (const auto& node : parser.graph.getVertexSet()){
        latitude1 = node->getAirport().getLatitude();
        longitude1 = node->getAirport().getLongitude();
        if (Graph::distance(latitude,longitude,latitude1,longitude1) <= radius)
            localAirports.push_back(node->getAirport().getCode());
    }
    return localAirports;
}
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
list<pair<string,string>> Utils::processFlight(int& bestFlight, const vector<string>& src, const vector<string>& dest,
                                               const Airline::AirlineH& airline) {
    bestFlight = INT_MAX;
    int nrFlights;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (s == d) continue;
            nrFlights = parser.graph.nrFlights(parser.idAirports[s], parser.idAirports[d], airline);
            if (nrFlights != 0 && nrFlights < bestFlight) {
                bestFlight = nrFlights;
                res.clear();
                res.emplace_back(s,d);
            }
            else if(nrFlights == bestFlight)
                res.emplace_back(s,d);
        }
    return res;
}
