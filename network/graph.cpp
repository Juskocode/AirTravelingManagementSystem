#include "graph.h"

#include <utility>
#include "../classes/Parser.h"

Graph::Graph(int Vertexes) {
    vertexSet.resize(Vertexes);
}


int Graph::getNumVertex() const {
    return (int)vertexSet.size();
}

vector<Vertex*> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
bool Graph::findVertex(const int &in) const {

    if(vertexSet.empty())
        return false;

    return (in >= 0 && in < size);
}

bool Vertex::isVisited() const {
    return visited;
}

void Vertex::setVisited(bool v) {
    Vertex::visited = v;
}

const list<Edge> &Vertex::getAdj() const {
    return adj;
}

void Vertex::setAdj(const list<Edge> &adj) {
    Vertex::adj = adj;
}

double Vertex::getDistance() const{
    return distance;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */

bool Graph::addFlight(const int &src, const int &dest, const Airline &airline, double w) {
    if (!findVertex(src) || !findVertex(dest))
        return false;

    vertexSet[src]->addEdge(vertexSet[dest], airline, w);
    return true;
}

bool Graph::addAirport(const int &src, const Airport &airport) {
    vertexSet.push_back(new Vertex(src, airport));
    return !findVertex(src);
}

double Graph::distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formula
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}


int Graph::nrFlights(int src, int dest, Airline::AirlineH airlines){

    if(!findVertex(src) || !findVertex(dest))
        return {};

    for (int i = 0; i < getNumVertex(); i++) {
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = 0;
    }

    queue<Vertex*> q;
    q.push(vertexSet[src]);

    vertexSet[src]->visited = true;

    while(!q.empty()){

        Vertex* currV = q.front();q.pop();

        for(const Edge &e : currV->adj){

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            Vertex* w = e.getDest();

            if(!vertexSet[w->getId()]->isVisited()){
                q.push(w);
                w->setVisited(true);
                w->distance = currV->distance + 1;
            }
        }
    }

    return (int)vertexSet[dest]->distance;
}

int Graph::airlineFlights(const string& airline){
    int count = 0;
    for (int i = 0 ; i < getNumVertex(); i++)
        for (const Edge& e : vertexSet[i]->getAdj())
            if (e.airline.getCode() == airline)
                count++;
    return count;
}

vector<string> Graph::getAirlines(int src, int dest, Airline::AirlineH airlines) {
    vector<string> usedAirlines;
    for (const auto& e: vertexSet[src]->adj)
        if (e.dest->getId() == dest && (airlines.empty() || airlines.find(e.airline) != airlines.end()))
            usedAirlines.push_back(e.airline.getCode());
    return usedAirlines;
}

vector<pair<int, string>> Graph::flightsPerAirport() {
    vector<pair<int,string>> n;

    for (int i = 0; i < getNumVertex(); i++){
        int nrFlights = (int)vertexSet[i]->getAdj().size();
        n.emplace_back(nrFlights, vertexSet[i]->getAirport().getCode());
    }

    sort(n.begin(), n.end(), [](const pair<int,string>& a, const pair<int,string>& b)
                                                                            {return a.first > b.first;});
    return n;
}

vector<pair<int,string>> Graph::airlinesPerAirport() {

    vector<pair<int,string>> nrAirlines;

    for (int i = 0; i < getNumVertex(); i++){
        set<string> n;

        for (const Edge& e : vertexSet[i]->getAdj())
            n.insert(e.airline.getCode());

        nrAirlines.emplace_back(n.size(), vertexSet[i]->getAirport().getCode());
    }

    sort(nrAirlines.begin(), nrAirlines.end(), [](const pair<int,string>& a, const pair<int,string>& b)
                                                                                            {return a.first > b.first;});

    return nrAirlines;
}

Graph::PairH Graph::airportsFromAirport(int source) const {
    Graph::PairH ans;
    for(const auto& e : vertexSet[source]->getAdj())
        ans.insert({vertexSet[e.getDest()->getId()]->airport.getCode(),
                    vertexSet[e.getDest()->getId()]->airport.getName()});
    return ans;
}

unordered_set<string> Graph::airlinesFromAirport(int i) {
    unordered_set<string> ans;
    for (const Edge& e : vertexSet[i]->getAdj())
        ans.insert(e.airline.getCode());
    return ans;
}

unordered_set<string> Graph::countriesFromAirport(int i) const {
    unordered_set<string> ans;
    for (const Edge& e : vertexSet[i]->getAdj()){
        int w = e.dest->getId();
        ans.insert(vertexSet[w]->getAirport().getCountry());
    }
    return ans;
}


Airport::CityH2 Graph::targetsFromAirport(int i){
    Airport::CityH2 ans;
    for (const auto& e : vertexSet[i]->getAdj()){
        int w = e.dest->getId();
        ans.insert({vertexSet[w]->airport.getCountry(), vertexSet[w]->airport.getCity()});
    }
    return ans;
}



void Graph::bfsPath(int src, Airline::AirlineH airlines){

    if(!findVertex(src))
        return;

    for (int i = 0; i < getNumVertex(); i++) {
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->parents.clear();
    }

    queue<Vertex*> q;
    q.push(vertexSet[src]);

    vertexSet[src]->parents = {-1};
    vertexSet[src]->distance = 0;

    while(!q.empty()){

        Vertex* currV = q.front();q.pop();

        for(const Edge &e : currV->adj){

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            Vertex* w = e.getDest();

            if(vertexSet[w->getId()]->distance > vertexSet[currV->getId()]->distance + 1){
                vertexSet[w->getId()]->distance = vertexSet[currV->getId()]->distance + 1;
                q.push(w);
                vertexSet[w->getId()]->parents.clear();
                vertexSet[w->getId()]->parents.push_back(currV->getId());
            }
            else if(vertexSet[w->getId()]->distance == vertexSet[currV->getId()]->distance + 1)
                vertexSet[w->getId()]->parents.push_back(currV->getId());
        }
    }
}

void Graph::findPaths(vector<vector<int>>& paths,vector<int>& path, int v){

    if (!findVertex(v)) {
        if (find(paths.begin(),paths.end(),path) == paths.end())
            paths.push_back(path);
        return;
    }

    for (const auto &parent : vertexSet[v]->parents) {
        path.push_back(v);
        findPaths(paths,path, parent);
        path.pop_back();
    }
}


Vertex *Graph::dijkstra(int src, int dest, Airline::AirlineH airlines) {

    if(!findVertex(src) || !findVertex(dest))
        return {};

    //node id and node value(distance)
    MinHeap<int, double> minHeap(getNumVertex(), -1);

    for(int i = 0; i < getNumVertex(); i++){
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->setVisited(false);
        vertexSet[i]->parents.clear();
        minHeap.insert(i, INT_MAX);
    }

    vertexSet[src]->distance = 0;
    vertexSet[src]->parents.push_back(src);

    minHeap.decreaseKey(src, 0);

    while(!minHeap.empty()){

        auto u = minHeap.extractMin();
        vertexSet[src]->setVisited(true);

        for(const auto &e : vertexSet[u]->getAdj()){

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            auto v = e.getDest()->getId();
            double w = e.getWeight();

            if(!vertexSet[v]->isVisited() && vertexSet[u]->distance + w < vertexSet[v]->distance){

                vertexSet[v]->distance = vertexSet[u]->distance + w;

                auto p = vertexSet[u]->parents;
                if (find(p.begin(), p.end(), v) == p.end()) p.push_back(v);

                vertexSet[v]->parents = p;
                minHeap.decreaseKey(v, vertexSet[v]->distance);

            }
        }
    }

    return vertexSet[dest];
}

Vertex *Graph::dijkstraFib(int src, int dest, Airline::AirlineH airlines) {
    if(!findVertex(src) || !findVertex(dest))
        return {};

    //node id and node value(distance)
    FibTree<Vertex *> fibHeap;

    for(int i = 0; i < getNumVertex(); i++){
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->setVisited(false);
        vertexSet[i]->parents.clear();
        fibHeap.insert(vertexSet[i]);
    }

    vertexSet[src]->distance = 0;
    vertexSet[src]->parents.push_back(src);
    Vertex newKey(src);
    newKey = *vertexSet[src];
    newKey.distance = 0;
    fibHeap.decreaseKey(vertexSet[src], &newKey);

    while(!fibHeap.empty()){

        auto u = fibHeap.extractMin()->getId();
        vertexSet[src]->setVisited(true);

        for(const auto &e : vertexSet[u]->getAdj()){

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            auto v = e.getDest()->getId();
            double w = e.getWeight();

            if(!vertexSet[v]->isVisited() && vertexSet[u]->distance + w < vertexSet[v]->distance){

                Vertex newKey(v);
                vertexSet[v]->distance = vertexSet[u]->distance + w;

                auto p = vertexSet[u]->parents;
                if (find(p.begin(), p.end(), v) == p.end()) p.push_back(v);

                vertexSet[v]->parents = p;

                newKey = *vertexSet[v];
                fibHeap.decreaseKey(vertexSet[v], &newKey);

            }
        }
    }

    return vertexSet[dest];
}


double Graph::bfsDiameter(int v) {
    for(int i = 0; i < getNumVertex(); i++){
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = -1.0;
    }

    queue<int> q;
    q.push(v);
    vertexSet[v]->setVisited(true);
    vertexSet[v]->distance = 0.0;
    double max = 0;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const auto& e: vertexSet[u]->getAdj()){

            int w = e.dest->getId();

            if (!vertexSet[w]->isVisited()){
                q.push(w);
                vertexSet[w]->setVisited(true);
                vertexSet[w]->distance = vertexSet[u]->distance + 1;
                if (vertexSet[w]->distance > max) max = vertexSet[w]->distance;
            }
        }
    }
    return max;
}


double Graph::diameter() {
    vertexSet[0]->setVisited(true);
    double max = bfsDiameter(0);
    for(int i = 0; i < getNumVertex(); i++)
        if (!vertexSet[i]->isVisited()){
            vertexSet[i]->setVisited(true);
            double diameter = bfsDiameter(i);
            if (diameter > max) max = diameter;
        }
    return max;
}

//TODO check for all cases
void Graph::dfsArt(int v, int index, list<int>& res, Airline::AirlineH airlines, int parent) {
    vertexSet[v]->num = vertexSet[v]->low = index++;
    vertexSet[v]->art = false; // Initialize as false, then set to true if it's an articulation point
    int count = 0;
    bool isArticulationPoint = false;

    for (const auto& e : vertexSet[v]->getAdj()) {
        auto w = e.dest->getId();

        if (airlines.find(e.airline) != airlines.end() || airlines.empty()) {
            if (vertexSet[w]->num == 0) {
                count++;
                dfsArt(w, index, res, airlines, v);
                vertexSet[v]->low = min(vertexSet[v]->low, vertexSet[w]->low);

                if ((vertexSet[w]->low >= vertexSet[v]->num && parent != -1) || (parent == -1 && count > 1)) {
                    vertexSet[v]->art = true;
                    isArticulationPoint = true;
                }
            } else if (w != parent) { // Ignore edge to the parent in DFS tree
                vertexSet[v]->low = min(vertexSet[v]->low, vertexSet[w]->num);
            }
        }
    }

    if ((parent == -1 && count > 1) || (parent != -1 && isArticulationPoint)) {
        res.push_back(v);
    }
}

list<int> Graph::articulationPoints(const Airline::AirlineH& airlines) {
    list<int> res;

    for(int i = 0; i < getNumVertex(); i++){
        vertexSet[i]->setVisited(false);
        vertexSet[i]->art = false;
    }

    int index = 0;

    for(int i = 0; i < getNumVertex(); i++)
        if (vertexSet[i]->num == 0)
            dfsArt(i, index,res, airlines, 0);

    return res;
}


void Graph::printPath(vector<int> path, const Airline::AirlineH& airlines) {
    for (int i = 0; i < path.size() - 1; i++){
        auto possibleAirlines = getAirlines(path[i],path[i+1],airlines);
        printf("\033[1m\033[46m %s \033[0m", vertexSet[path[i]]->airport.getCode().c_str());
        cout <<" --- (";
        for (int j = 0; j < possibleAirlines.size() - 1; j++)
            printf("\033[1m\033[32m %s \033[0m |",possibleAirlines[j].c_str());
        printf("\033[1m\033[32m %s \033[0m",possibleAirlines[possibleAirlines.size()-1].c_str());
        cout << ") --- ";
    }
    printf("\033[1m\033[46m %s \033[0m\n\n", vertexSet[path[path.size() - 1]]->airport.getCode().c_str());
}


void Graph::printPathsByFlights(int& nrPath, int start, int end, const Airline::AirlineH& airlines) {
    vector<int> path;
    vector<vector<int> > paths;

    bfsPath(start,airlines);
    findPaths(paths,path,end);

    Parser parser;
    auto map = parser.getMap();
    for (auto v : paths) {
        reverse(v.begin(), v.end());
        cout << " Trajeto nº" << ++nrPath << ": ";
        printPath(v,airlines);
    }
}


void Graph::printPathsByDistance(int& nrPath, int start, int end, const Airline::AirlineH& airlines) {
    Vertex* v = dijkstra(start,end,airlines);

    if (v->parents.empty()) {
        cout << " Não existem voos\n\n";
        return;
    }

    cout << " Trajeto nº" << ++nrPath << ": ";
    printPath(v->parents, airlines);

}