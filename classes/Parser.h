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
private:
    void createAirports();
    void createAirlines();
    void createGraph();

    Airport::AirportH airports;
    Airline::AirlineH airlines;
    Graph<int> graph;

};


#endif //AIRBUSMANAGEMENTSYSTEM_PARSER_H
