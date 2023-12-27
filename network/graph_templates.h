#ifndef AIRBUSMANAGEMENTSYSTEM_GRAPH_TEMPLATES_H
#define AIRBUSMANAGEMENTSYSTEM_GRAPH_TEMPLATES_H

#include "graph.h"

/**
 * @file
 * @brief Contains Graph template functions implementations
 */
template<typename Container>
Container Graph::listReachableEntities(int v, int max) {
    for (int i = 0; i < getNumVertex(); i++)
        vertexSet[i].setVisited(false);

    Container entities;

    queue<int> q;
    q.push(v);
    vertexSet[v].setVisited(true);
    vertexSet[v].distance = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : vertexSet[u].getAdj()) {

            int w = e.dest;

            if (!vertexSet[w].isVisited()) {

                q.push(w);
                vertexSet[w].setVisited(true);
                vertexSet[w].distance = vertexSet[u].getDistance() + 1;

                if (vertexSet[w].getDistance() <= max) {

                    if constexpr (std::is_same<Container, Airport::AirportH>::value) {
                        entities.insert(vertexSet[w].getAirport());
                    }
                    else if constexpr (std::is_same<Container, Airport::CityH2>::value) {
                        entities.insert({vertexSet[w].getAirport().getCountry(), vertexSet[w].getAirport().getCity()});
                    }
                    else if constexpr (std::is_same<Container, std::set<std::string>>::value) {
                        entities.insert(vertexSet[w].getAirport().getCountry());
                    }
                }
            }
        }
    }
    return entities;
}

#endif //AIRBUSMANAGEMENTSYSTEM_GRAPH_TEMPLATES_H
