//
// Created by Utilizador on 03/12/2023.
//

#include "graph.h"


Vertex::Vertex(int id): id(id) {}

Vertex::Vertex(int id, Airport airport) : id(id), airport(std::move(airport)) {}

int Vertex::getId() const {
    return id;
}

Airport Vertex::getAirport() {
    return airport;
}

void Vertex::setAirport(Airport in) {
    Vertex::airport = in;
}

bool Vertex::isProcessing() const {
    return processing;
}

void Vertex::setProcessing(bool p) {
    Vertex::processing = p;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
void Vertex::addEdge(Vertex *d, const Airline& airline, double w) {
    adj.emplace_back(d, airline, w);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex::removeEdgeTo(Vertex *d) {
    for (auto it = adj.begin(); it != adj.end(); it++)
        if (it->dest  == d) {
            adj.erase(it);
            return true;
        }
    return false;
}


