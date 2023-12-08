#include "Parser.h"

Parser::Parser() {
    createAirports();
    createAirlines();
    createGraph();
}

Airport::AirportH const& Parser::getAirports() const {return airports;}
Airline::AirlineH const& Parser::getAirlines() const {return airlines;}
Airport::CityH const& Parser::getCity() const {return airportsPerCity;}
Graph Parser::getGraph() const {return graph;}


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

        graph.addEdge(idAirports[source],idAirports[target],Airline(airline),d);
    }
}
