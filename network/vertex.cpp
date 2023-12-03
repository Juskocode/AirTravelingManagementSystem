//
// Created by Utilizador on 03/12/2023.
//

#include "graph.h"


Vertex::Vertex(Airport in): airport(in) {}

string Vertex::getAirport() const {
    return airport.getName();
}

void Vertex::setInfo(T in) {
    Vertex::info = in;
}

bool Vertex<T>::isProcessing() const {
    return processing;
}

void Vertex<T>::setProcessing(bool p) {
    Vertex::processing = p;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
void Vertex<T>::addEdge(Vertex<T> *d, Airline airline, double w) {
    adj.push_back(Edge<T>(d, airline, Airline(__cxx11::basic_string())));
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for (auto it = adj.begin(); it != adj.end(); it++)
        if (it->dest  == d) {
            adj.erase(it);
            return true;
        }
    return false;
}
