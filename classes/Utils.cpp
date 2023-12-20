
#include "Utils.h"

#include <utility>

Utils::Utils(){
    countAirportsPerCountry();
}



bool Utils::isCountry(const string& country){
    auto i = countries.find(country);
    if (i == countries.end()) return false;
    return true;
}

bool Utils::isAirport(const Airport& airport){
    auto i = airports.find(airport);
    if (i == airports.end()) return false;
    return true;
}

bool Utils::isAirline(const Airline& airline) {
    auto i = airlines.find(airline);
    if (i == airlines.end()) return false;
    return true;
}

bool Utils::isCity(const string& city) {
    auto i = cities.find(city);
    if (i == cities.end()) return false;
    return true;
}

bool Utils::isValidCity(const string& country, const string& city) {
    for (const auto& c : citiesPerCountry[country])
        if (city == c) return true;
    return false;
}

int Utils::nrFlights(){
    int nrFlights = 0;
    for (const auto& node: graph.getVertexSet())
        nrFlights += node->getAdj().size();
    return nrFlights;
}


vector<string> Utils::localAirports(double latitude, double longitude, double radius) const {
    vector<string> localAirports;
    double latitude1,longitude1;
    for (const auto& node : graph.getVertexSet()){
        latitude1 = node->getAirport().getLatitude();
        longitude1 = node->getAirport().getLongitude();
        if (Graph::distance(latitude,longitude,latitude1,longitude1) <= radius)
            localAirports.push_back(node->getAirport().getCode());
    }
    return localAirports;
}

list<pair<string,string>> Utils::processFlight(int& bestFlight, const vector<string>& src, const vector<string>& dest,
                                               const Airline::AirlineH& airline) {
    bestFlight = INT_MAX;
    int nrFlights;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (s == d) continue;
            nrFlights = graph.nrFlights(idAirports[s], idAirports[d], airline);
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

list<pair<string,string>> Utils::processDistance(double& bestDistance, const vector<string>& src, const vector<string>& dest,
                                                      const Airline::AirlineH& airline) {
    bestDistance = MAXFLOAT;
    double distance;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (s == d) continue;
            auto node = graph.dijkstra(idAirports[s], idAirports[d], airline);
            distance = node->getDistance();
            if (distance < bestDistance) {
                bestDistance = distance;
                res.clear();
                res.emplace_back(s,d);
            }
            else if (distance == bestDistance)
                res.emplace_back(s,d);
        }
    return res;
}


void Utils::countAirportsPerCountry() {
    map<string, int> airportsPerCountry;
    for (const auto& i : airportsPerCity){
        if (airportsPerCountry.find(i.first.first) == airportsPerCountry.end())
            airportsPerCountry[i.first.first] = i.second.size();
        else{
            auto m = airportsPerCountry.find(i.first.first);
            m->second += i.second.size();
        }
    }
    nrAirportsPerCountry = airportsPerCountry;
}

int Utils::countAirlinesPerCountry(const string& country) {
    int count = 0;
    for (auto airline : airlines)
        if (airline.getCountry() == country)
            count++;
    return count;
}

map<string,int> Utils::getNrAirportsPerCountry() const {return nrAirportsPerCountry;}
