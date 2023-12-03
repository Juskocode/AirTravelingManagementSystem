#ifndef AIRBUSMANAGEMENTSYSTEM_GRAPH_H
#define AIRBUSMANAGEMENTSYSTEM_GRAPH_H

#include <cstddef>
#include <vector>
#include <queue>
#include <cmath>
#include <list>
#include "../classes/airport.h"
#include "../classes/airline.h"

using namespace std;

class Edge;
class Vertex;



class Edge {
    Vertex * dest{};       // destination vertex
    double weight{};       // edge weight
    Airline airline;
public:
    Edge(Vertex *d, Airline airline, double w);
    Vertex *getDest() const;
    void setDest(Vertex *dest);
    double getWeight() const;
    void setWeight(double weight);
    friend class Graph;
    friend class Vertex;
};
class Vertex {
    Airport airport = Airport(""); // content
    vector<Edge > adj;    // list of outgoing edges
    bool visited{};          // auxiliary field
    bool processing{};       // auxiliary field
    int num = 0;
    int low{};
    double distance{};

    void addEdge(Vertex *dest, Airline airline, double w);
    bool removeEdgeTo(Vertex *d);
public:
    explicit Vertex(Airport airport);
    string getAirport() const;
    void setAirport(Airport in);
    bool isVisited() const;
    void setVisited(bool v);
    bool isProcessing() const;
    void setProcessing(bool p);
    const vector<Edge> &getAdj() const;
    void setAdj(const vector<Edge> &adj);
    friend class Graph;
};



class Graph {

    vector<Vertex *> vertexSet;    // vertex set
    void dfsVisit(Vertex *v,  vector<int> &res) const;

public:

    explicit Graph(int Vertexs);
    Vertex *findVertex(const Airport &in) const;

    bool addVertex(const int &src);
    bool removeVertex(const Airport &in);
    bool addEdge(const int &sourc, const int &dest, const Airline &airline, double w);
    bool removeEdge(const int &sourc, const int &dest);
    bool addAirport(const int &sourc, const Airport &airport);

    int getNumVertex() const;
    static double distance(double lat1, double lon1, double lat2, double lon2);

    vector<Vertex * > getVertexSet() const;
    vector<int> dfs() const;
    vector<int> dfs(const int &source) const;
    vector<int> bfs(const int &source) const;
};




#endif //AIRBUSMANAGEMENTSYSTEM_GRAPH_H
