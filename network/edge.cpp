//
// Created by Utilizador on 03/12/2023.
//

#include <utility>

#include "graph.h"

/**
 * @file
 * @brief Contains the Edge class implementation
 */

Edge::Edge(Vertex *d, Airline airline, double w) : dest(d), weight(w), airline(std::move(airline)) {}

Vertex *Edge::getDest() const {
    return dest;
}

double Edge::getWeight() const {
    return weight;
}

Airline Edge::getAirline() const{
    return airline;
}
