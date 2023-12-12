#include "Parser.h"
#include <chrono>

Parser::Parser() {
    auto start = std::chrono::steady_clock::now();
    createAirports();
    createAirlines();
    createGraph();
    auto end = std::chrono::steady_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output the duration
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
}

Airport::AirportH const& Parser::getAirports() const {return airports;}
Airline::AirlineH const& Parser::getAirlines() const {return airlines;}
Airport::CityH const& Parser::getCity() const {return airportsPerCity;}
Graph Parser::getGraph() const {return graph;}
unordered_map<string,int> Parser::getMap() const{
    return idAirports;
}

/**
 * Swaps key and value.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @tparam A - key
 * @tparam B - value
 * @param p
 * @return pair where key is the value(B) and value is the key(A)
 */
template<typename A, typename B>
pair<B,A> flip_pair(const pair<A,B> &p)
{
    return pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
multimap<B,A> flip_map(const map<A,B> &src)
{
    multimap<B,A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()),
              flip_pair<A,B>);
    return dst;
}

multimap<int,string> Parser::convertMap(const map<string, int>& m) {
    return flip_map(m);
}

map<string,int> Parser::getNrAirportsPerCountry() const {return nrAirportsPerCountry;}


void Parser::createAirports() {
    ifstream myFile;
    string currentLine, code, name, city, country, x;
    double latitude, longitude;
    int i = 1;
    myFile.open("../data/airports.csv");
    getline(myFile, currentLine);

    while (getline(myFile,currentLine)){
        istringstream iss(currentLine);

        getline(iss,code,',');
        getline(iss,name,',');
        getline(iss,city,',');
        getline(iss,country,',');
        getline(iss,x,','); latitude = stod(x);
        getline(iss,x,','); longitude = stod(x);

        Airport airport = Airport(code,name,city,country,latitude,longitude);
        graph.addAirport(i,airport);
        airportsPerCity[{airport.getCountry(),airport.getCity()}].push_back(code);
        idAirports.insert({airport.getCode(),i++});
        airports.insert(airport);
        countries.insert(country);
        cities.insert(city);
        citiesPerCountry[country].push_back(city);
    }
}

void Parser::createAirlines() {
    ifstream inFile;
    string code, name, callSign, country, line;
    inFile.open("../data/airlines.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,code,',');
        getline(is,name,',');
        getline(is,callSign,',');
        getline(is,country,',');
        Airline a = Airline(code, name, callSign, country);
        airlines.insert(a);
    }
}

void Parser::createGraph(){
    ifstream inFile;
    string source, target, airline, line;
    inFile.open("../data/flights.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,source,',');
        getline(is,target,',');
        getline(is,airline,',');

        double latSource = airports.find(Airport(source))->getLatitude();
        double lonSource = airports.find(Airport(source))->getLongitude();

        double latTarget = airports.find(Airport(target))->getLatitude();
        double lonTarget = airports.find(Airport(target))->getLongitude();

        auto d = Graph::distance(latSource, lonSource, latTarget, lonTarget);

        graph.addFlight(idAirports[source],idAirports[target],Airline(airline),d);
    }
}
