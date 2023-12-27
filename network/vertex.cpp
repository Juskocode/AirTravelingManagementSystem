//
// Created by Utilizador on 03/12/2023.
//

#include "graph.h"

/**
 * @file
 * @brief Contains the Vertex class implementation
 */
Vertex::Vertex(int id): id(id) {}

Vertex::Vertex(int id, Airport airport) : id(id), airport(std::move(airport)) {}

int Vertex::getId() const {
    return id;
}

Airport Vertex::getAirport() {
    return airport;
}

bool Vertex::isProcessing() const {
    return processing;
}

void Vertex::setProcessing(bool p) {
    Vertex::processing = p;
}


void Vertex::addEdge(const int d, const Airline& airline, double w) {
    adj.emplace_back(d, airline, w);
}


