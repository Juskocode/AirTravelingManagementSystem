//
// Created by Utilizador on 03/12/2023.
//

#include "graph.h"

Edge::Edge(Vertex *d, double w) : dest(d), weight(w), airline("") {}

Vertex *Edge::getDest() const {
    return dest;
}


void Edge::setDest(Vertex *d) {
    Edge::dest = d;
}

double Edge::getWeight() const {
    return weight;
}

void Edge::setWeight(double weight) {
    Edge::weight = weight;
}
