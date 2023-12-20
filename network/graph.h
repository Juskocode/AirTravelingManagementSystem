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


class Edge;
class Vertex;
class Graph;


class Edge {
    Vertex * dest{};       // destination vertex
    double weight{};       // edge weight
    Airline airline;
public:
    Edge(Vertex *d, Airline airline, double w);
    [[nodiscard]] Vertex *getDest() const;
    void setDest(Vertex *dest);
    [[nodiscard]] double getWeight() const;
    void setWeight(double weight);
    [[nodiscard]] Airline getAirline() const;
    friend class Graph;
    friend class Vertex;
};
class Vertex {
    int id;
    Airport airport = Airport(""); // content
    list<Edge > adj;    // list of outgoing edges
    bool visited{};          // auxiliary field
    bool processing{};       // auxiliary field
    int num = 0;             // to use in articulation points
    int low{};               // to use in articulation points
    bool art;                // to use in articulation points
    double distance{};
    vector<int> parents; //to use in bfsPath

    void addEdge(Vertex *dest, const Airline& airline, double w);
    bool removeEdgeTo(Vertex *d);
public:
    explicit Vertex(int id);
    Vertex(int id, Airport airport);
    [[nodiscard]] int getId() const;
    [[nodiscard]] double getDistance() const;
    Airport getAirport();
    void setAirport(Airport in);
    [[nodiscard]] bool isVisited() const;
    void setVisited(bool v);
    [[nodiscard]] bool isProcessing() const;
    void setProcessing(bool p);
    [[nodiscard]] const list<Edge> &getAdj() const;
    void setAdj(const list<Edge> &adj);
    friend class Graph;

};



class Graph {

    vector<Vertex *> vertexSet;    // vertex set
    const int size = 3019;

public:

    explicit Graph(int vertexes);
    [[nodiscard]] bool findVertex(const int &in) const;

    bool addFlight(const int &src, const int &dest, const Airline &airline, double w);
    bool addAirport(const int &src, const Airport &airport);

    [[nodiscard]] int getNumVertex() const;
    static double distance(double lat1, double lon1, double lat2, double lon2);

    [[nodiscard]] vector<Vertex * > getVertexSet() const;

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
     * Calculates the number of flights of a specific airline\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b>, E -> number of edges
     * </pre>
     * @param airline
     * @return number of flights of a specific airline
     */
    int airlineFlights(const string& airline);

    /**
     * Calculates the number of departures of each airport.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|*log(|V|))</b>, V -> number of nodes
     * </pre>
     * @return ordered vector of pair<Number of departures,Airport Code> by descending order of number of flights
     */
    vector<pair<int, string>> flightsPerAirport();

    /**
     * Calculates the number of airlines that work with each airport.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V| + |E| + n * log(n))</b>, V -> number of nodes, E -> number of edges, n -> size of nrAirlines
     * </pre>
     * @return ordered vector of pair<Number of airlines,Airport Code> by descending order of number of airlines
     */
    vector<pair<int, string>> airlinesPerAirport();


    /*!                                                 */
    /*!                 Main Algorithms                 */
    /*!                                                 */


    /**
    * Calculates the reachable entities using "max" number of flights\n\n
    * <b>Complexity\n</b>
    * <pre>
    *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
    * </pre>
    * @param v - source node
    * @param max - number of flights
    * @note the entities can be airports, countries or cities
    * @return set of reachable airports using "max" number of flights
    * @example  Airport::AirportH airports = graph.listReachableEntities<Airport::AirportH>(sourceNode, maxFlights);
    *           Airport::CityH2 cities = graph.listReachableEntities<Airport::CityH2>(sourceNode, maxFlights);\n
    *           std::set<std::string> countries = graph.listReachableEntities<std::set<std::string>>(sourceNode, maxFlights);
    */
    template <typename Container>
    Container listReachableEntities(int v, int max);

    struct PairStringHash{
        int operator()(const pair<string,string> &b) const {
            string code = b.second;
            int v = 0;
            for (char i : code)
                v = 37 * v + i;
            return v;
        }
        bool operator()(const pair<string,string> &b1, const pair<string,string> &b2) const {
            return b1.first == b2.first && b1.second== b2.second;
        }
    };
    typedef unordered_set<pair<string, string>, PairStringHash, PairStringHash> PairH;

    /**
     * From a specific airport, calculates all of the airports that are reachable within 1 flight.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b>,E -> number of edges of source node
     * </pre>
     * @param source - source node
     * @return unordered_set of airports code and name
     */
    PairH airportsFromAirport(int source) const;

    /**
     * Calculates the different airlines that cooperate with an airport\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b>, E -> number of edges of node i
     * </pre>
     * @param i - source node
     * @return set of all the different airlines
     */
    unordered_set<string> airlinesFromAirport(int i);

    /**
     * Calculates the different cities that are reachable from an airport within 1 flight\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b>, E -> number of edges of node i
     * </pre>
     * @param i - source node
     * @return set of all the different cities
     */
    Airport::CityH2 targetsFromAirport(int i);

    /**
     * Calculates the different countries that are reachable from an airport within 1 flight\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|E|)</b>, E -> number of edges of node i
     * </pre>
     * @param i - source node
     * @return set of all the different countries
     */
    unordered_set<string> countriesFromAirport(int i) const;

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
     *      <b>O(n)</b> n -> paths vector size
     * </pre>
     * @param paths - vector of paths that are possible
     * @param path - current path
     * @param v - target node
     */
    void findPaths(vector<vector<int>>& paths, vector<int>& path, int v);

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

    Vertex* dijkstraFib(int src, int dest, Airline::AirlineH airlines);


    /**
     * Calculates the max distance between connected nodes\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
     * </pre>
     * @param v - source node
     * @return the diameter of a connected component
     */
    double bfsDiameter(int v);

    /**
     * Calculates the diameter using a bfs.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
     * </pre>
     * @return diameter between all connected components.
     */
    double diameter();

    /**
     * Finds the nodes that are articulation points and inserts them in res\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O((|V| + |E|)*n)</b>, V -> number of nodes, E -> number of edges, n-> list size
     * </pre>
     * @param v - source node
     * @param index
     * @param res - list of articulation points
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void dfsArt(int v, int index, list<int> &res, Airline::AirlineH airlines, int parent);

    /**
     * Calculates the list of articulation points that exist in a specific unordered_set of airlines or in all airlines.\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O((|V| + |E|) * n)</b>, V -> number of nodes, E -> number of edges, n-> list size
     * </pre>
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     * @return The list of articulation points.
     */
    list<int> articulationPoints(const Airline::AirlineH& airlines);

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
     *      <b>O(n * m)</b>, n -> path size , m -> possibleAirlines size
     * </pre>
     * @param path - visited nodes during the path
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void printPath(vector<int>,const Airline::AirlineH&);

    /**
     * Calculates (using bfs) and prints most optimal path of flights(least amount of flights)\n\n
     * <b>Complexity\n</b>
     * <pre>
     *      <b>O((|V|+|E|) * p)</b>, V -> number of nodes , E-> number of edges, p-> possibleAirlines size
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
     *      <b>O(log(|V|) * p)</b>, V -> number of nodes , p -> possibleAirlines size
     * </pre>
     * @param nrPath
     * @param start - source node
     * @param end  - final node
     * @param airlines - unordered set of airlines to use (if empty, use all airlines)
     */
    void printPathsByDistance(int& nrPath, int start, int end, const Airline::AirlineH& airlines);

};



#include "graph_templates.h"
#endif //AIRBUSMANAGEMENTSYSTEM_GRAPH_H
