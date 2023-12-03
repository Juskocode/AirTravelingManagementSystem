#ifndef AIRBUSMANAGEMENTSYSTEM_GRAPH_H
#define AIRBUSMANAGEMENTSYSTEM_GRAPH_H

#include <cstddef>
#include <vector>
#include <queue>
#include <cmath>
#include "../classes/airport.h"
#include "../classes/airline.h"

using namespace std;

template <class T> class Vertex;
template <class T> class Edge;
template <class T> class Graph;

template <class T>
class Edge {
    Vertex<T> * dest{};       // destination vertex
    double weight{};          // edge weight
    Airline airline;
public:
    Edge(Vertex<T> *d, double w);
    Vertex<T> *getDest() const;
    void setDest(Vertex<T> *dest);
    double getWeight() const;
    void setWeight(double weight);
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
class Vertex {
    T info;                // contents
    Airport airport = Airport("");
    vector<Edge<T> > adj;    // list of outgoing edges
    bool visited{};          // auxiliary field
    bool processing{};       // auxiliary field
    int num = 0;
    int low{};
    double distance{};

    void addEdge(Vertex<T> *dest, Airline airline, double w);
    bool removeEdgeTo(Vertex<T> *d);
public:
    explicit Vertex(T in);
    T getInfo() const;
    void setInfo(T in);
    bool isVisited() const;
    void setVisited(bool v);
    bool isProcessing() const;
    void setProcessing(bool p);
    const vector<Edge<T>> &getAdj() const;
    void setAdj(const vector<Edge<T>> &adj);
    friend class Graph<T>;
};

template <class T>
class Graph {

    vector<Vertex<T> *> vertexSet;    // vertex set
    void dfsVisit(Vertex<T> *v,  vector<T> &res) const;

public:

    Vertex<T> *findVertex(const T &in) const;

    bool addVertex(const T &src);
    bool removeVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, const Airline &airline, double w);
    bool removeEdge(const T &sourc, const T &dest);
    bool addAirport(const T &sourc, const Airport &airport);

    int getNumVertex() const;
    static double distance(double lat1, double lon1, double lat2, double lon2);

    vector<Vertex<T> * > getVertexSet() const;
    vector<T> dfs() const;
    vector<T> dfs(const T &source) const;
    vector<T> bfs(const T &source) const;
};



#endif //AIRBUSMANAGEMENTSYSTEM_GRAPH_H
