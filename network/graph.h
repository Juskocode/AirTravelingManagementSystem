#ifndef AIRBUSMANAGEMENTSYSTEM_GRAPH_H
#define AIRBUSMANAGEMENTSYSTEM_GRAPH_H


#include <string>
#include <vector>
#include <unordered_set>
#include <list>
#include <queue>
#include <iostream>
#include <stack>
#include <cmath>
#include <set>
#include <algorithm>
#include <climits>
#include <utility>
#include "../classes/airport.h"
#include "../classes/airline.h"
#include "../classes/Fibtree.h"
#include "../classes/Minheap.h"


using namespace std;

class Edge;
class Vertex;
class Graph;


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
    Airline getAirline() const;
    friend class Graph;
    friend class Vertex;
};
class Vertex {
    int id;
    Airport airport = Airport(""); // content
    vector<Edge > adj;    // list of outgoing edges
    bool visited{};          // auxiliary field
    bool processing{};       // auxiliary field
    int num = 0;
    int low{};
    double distance{};
    vector<int> parents; //to use in bfsPath

    void addEdge(Vertex *dest, Airline airline, double w);
    bool removeEdgeTo(Vertex *d);
public:
    explicit Vertex(int id);
    int getId();
    double getDistance();
    Airport getAirport();
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
    Vertex *findVertex(const int &in) const;

    bool addVertex(const int &src);
    bool removeVertex(const int &in);
    bool addEdge(const int &sourc, const int &dest, const Airline &airline, double w);
    bool removeEdge(const int &sourc, const int &dest);
    bool addAirport(const int &sourc, const Airport &airport);

    int getNumVertex() const;
    static double distance(double lat1, double lon1, double lat2, double lon2);

    vector<Vertex * > getVertexSet() const;
    vector<int> dfs() const;
    vector<int> dfs(const int &source) const;
    vector<int> bfs(const int &source) const;

    /**
     * Calculates the minimum number of flights between source airport and target airport using airlines \n \n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
     * </pre>
     * @param src - source node / node of source airport
     * @param dest - target node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     * @return minimum number of flights between source airport and target airport using airlines
     */
    int nrFlights(int src, int dest, Airline::AirlineH airlines);

    /**
     * Stores in the parents variable the possible flight candidates using bfs. Parents who also have possible flight candidates
     * allowing us to get all the possible flights from a certain source.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
     * </pre>
     * @param src - source node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void bfsPath(int src, Airline::AirlineH airlines);

    /**
     * Stores in paths all possible paths to node v.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n)</b> n -> paths size
     * </pre>
     * @param paths - vector of paths that are possible
     * @param path - current path
     * @param v - target node
     */
    void findPaths(vector<vector<int>>& paths,vector<int>& path,int v);

    /**
     * Calculates the minimum flown distance between source airport and target airport using airlines \n \n
     * <b>Complexity\n</b>
     * <pre>

     *      <b>O(|E|log(|V|))</b>, V -> number of nodes and E is the number of Edges
     * </pre>
     * @param src - source node / node of source airport
     * @param dest - target node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     * @return minimum flown distance between source airport and target airport using airlines
     */
    Vertex* dijkstra(int src, int dest, Airline::AirlineH airlines);

    /**
     * Searches all the airlines that can be used to travel between a source and dest with a certain user input of airlines(or none).\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b> E -> number of edges of source node
     * </pre>
     * @param src - source node
     * @param dest - final node
     * @param airlines - unordered_set of airlines that without user input is empty, if has user input only uses those specific airlines.
     * @return vector of the possible airlines to use to travel from src to dest
     */
    vector<string> getAirlines(int src, int dest, Airline::AirlineH airlines);

    /**
     * Prints a possible path from a source airport to a target airport\n\n
     *  * <b>Complexity\n</b>
     * <pre>
     *      <b>O(n*m)</b>, n -> path size , m -> possibleAirlines size
     * </pre>
     * @param path - visited nodes during the path
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void printPath(vector<int>,const Airline::AirlineH&);

    /**
     * Calculates (using bfs) and prints most optimal path of flights(least amount of flights)\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O((|V|+|E|)*p)</b>, V -> number of nodes , E-> number of edges, p-> possibleAirlines size
     * </pre>
     * @param nrPath
     * @param start - source node
     * @param end - final node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void printPathsByFlights(int& nrPath, int start, int end, const Airline::AirlineH& airlines);

    /**
     * Calculates and prints the most optimal paths based on distance of nodes using the dijkstra algorithm.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(log(|V|)*p)</b>, V -> number of nodes , p -> possibleAirlines size
     * </pre>
     * @param nrPath
     * @param start - source node
     * @param end  - final node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void printPathsByDistance(int& nrPath, int start, int end, const Airline::AirlineH& airlines);

};




#endif //AIRBUSMANAGEMENTSYSTEM_GRAPH_H
