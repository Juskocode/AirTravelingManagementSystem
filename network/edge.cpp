//
// Created by Utilizador on 03/12/2023.
//

#include <utility>

#include "graph.h"

/**
 * @file
 * @brief Contains the Edge class implementation
 */

Edge::Edge(int dest, Airline airline, double w) : dest(dest), weight(w), airline(std::move(airline)) {}

int Edge::getDest() const {
    return dest;
}

double Edge::getWeight() const {
    return weight;
}

Airline Edge::getAirline() const{
    return airline;
}
