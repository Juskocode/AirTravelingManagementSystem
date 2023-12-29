#include "graph.h"

#include <utility>
#include "../classes/Parser.h"

/**
 * @file
 * @brief Contains the Graph class implementaion
 */
Graph::Graph(int Vertexes) {
    for (int i = 0; i < size; ++i) {
        vertexSet.push_back(new Vertex(i)); // Assuming the Vertex constructor with just id parameter
    }
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
    if(!findVertex(src))
        return false;
    vertexSet[src]->airport = airport;
    return true;
}

double Graph::haversineDistanceGeneric(double lat1, double lon1, double lat2, double lon2){
    constexpr double M_PI_180 = 0.017453292519943295; // Precomputed value of PI / 180

    double dLat = (lat2 - lat1) * M_PI_180;
    double dLon = (lon2 - lon1) * M_PI_180;

    lat1 *= M_PI_180;
    lat2 *= M_PI_180;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat1) * std::cos(lat2);

    double rad = 6371; // Earth's radius in kilometers
    double c = 2 * std::asin(std::sqrt(a));
    return rad * c;
}

// Function to compute distance using the Haversine formula
double Graph::haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    constexpr double M_PI_180 = 0.017453292519943295; // Precomputed value of PI / 180

    double dLat = (lat2 - lat1) * M_PI_180;
    double dLon = (lon2 - lon1) * M_PI_180;

    lat1 *= M_PI_180;
    lat2 *= M_PI_180;

    __m128d sin_dLat_dLon, cos_lat1, cos_lat2;

    sin_dLat_dLon = _mm_set_pd(sin(dLat / 2), sin(dLon / 2));
    cos_lat1 = _mm_set1_pd(cos(lat1));
    cos_lat2 = _mm_set1_pd(cos(lat2));

    __m128d cos_dLat = _mm_mul_pd(cos_lat1, cos_lat2);
    __m128d sin_dLat_sqr = _mm_mul_pd(sin_dLat_dLon, sin_dLat_dLon);
    __m128d a = _mm_add_pd(sin_dLat_sqr, cos_dLat);

    alignas(16) double a_arr[2];
    _mm_store_pd(a_arr, a);

    double rad = 6371; // Earth's radius in kilometers
    double c = 2 * asin(sqrt(a_arr[0])); // Square root for first element only

    return rad * c;
}

// Function to compute distance using Haversine formula in parallel
double Graph::parallelHaversineDistance_(double lat1, double lon1, double lat2, double lon2) {
    constexpr int numThreads = 6; // Maximum number of threads (cores)
    std::vector<std::thread> threads;
    std::vector<double> results(numThreads, 0.0);
    std::mutex mutex;

    // Split the workload across threads
    double latDiff = (lat2 - lat1) / numThreads;
    double lonDiff = (lon2 - lon1) / numThreads;

    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&results, &mutex, lat1, lon1, latDiff, lonDiff, i]() {
            double localLat1 = lat1 + latDiff * i;
            double localLon1 = lon1 + lonDiff * i;
            double localLat2 = localLat1 + latDiff;
            double localLon2 = localLon1 + lonDiff;

            double result = haversineDistance(localLat1, localLon1, localLat2, localLon2);

            std::lock_guard<std::mutex> lock(mutex);
            results[i] = result;
        });
    }

    // Wait for threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Calculate total distance from all segments
    double totalDistance = 0.0;
    for (const auto& result : results) {
        totalDistance += result;
    }

    return totalDistance;
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

Vertex* Graph::aStar(int src, int dest, Airline::AirlineH airlines) {
    //src and dest are prev verified

    // MinHeap with additional priority based on heuristic (Haversine distance)
    MinHeap<int, double> minHeap(getNumVertex(), -1);

    for (int i = 0; i < getNumVertex(); i++) {
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->setVisited(false);
        vertexSet[i]->parents.clear();
        minHeap.insert(i, INT_MAX);
    }

    vertexSet[src]->distance = 0;
    vertexSet[src]->parents.push_back(src);

    minHeap.decreaseKey(src, 0);

    while (!minHeap.empty()) {
        auto u = minHeap.extractMin();
        vertexSet[u]->setVisited(true);

        for (const auto &e : vertexSet[u]->getAdj()) {
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end())
                continue;

            auto v = e.getDest()->getId();
            double w = e.getWeight();

            if (!vertexSet[v]->isVisited() && vertexSet[u]->distance + w < vertexSet[v]->distance) {
                double newDistance = vertexSet[u]->distance + w;
                double heuristic = haversineDistance(vertexSet[v]->getAirport().getLatitude(),
                                                     vertexSet[v]->getAirport().getLongitude(),
                                                     vertexSet[dest]->getAirport().getLatitude(),
                                                     vertexSet[dest]->getAirport().getLongitude());


                vertexSet[v]->distance = newDistance;

                auto p = vertexSet[u]->parents;
                if (find(p.begin(), p.end(), v) == p.end()) p.push_back(v);
                vertexSet[v]->parents = p;

                double priority = newDistance + heuristic;
                minHeap.decreaseKey(v, priority);

            }
        }
    }

    return vertexSet[dest];
}

int Graph::bfsDiameter(int v) {
    for(int i = 0; i < getNumVertex(); i++){
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = -1.0;
    }

    queue<int> q;
    q.push(v);
    vertexSet[v]->setVisited(true);
    vertexSet[v]->distance = 0.0;
    int max = 0;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const auto& e: vertexSet[u]->getAdj()){

            int w = e.dest->getId();

            if (!vertexSet[w]->isVisited()){
                q.push(w);
                vertexSet[w]->setVisited(true);
                vertexSet[w]->distance = vertexSet[u]->distance + 1;
                if (vertexSet[w]->distance > max) max = (int)vertexSet[w]->distance;
            }
        }
    }
    return max;
}


int Graph::diameter() {
    vertexSet[0]->setVisited(true);
    int max = bfsDiameter(0);
    for(int i = 0; i < getNumVertex(); i++)
        if (!vertexSet[i]->isVisited()){
            vertexSet[i]->setVisited(true);
            int diameter = bfsDiameter(i);
            if (diameter > max) max = diameter;
        }
    return max;
}

vector<pair<string, string>> Graph::maxDiameterSourceDestPairs(int &d) {
    d = diameter(); // Calculate the maximum diameter

    vector<pair<string, string>> maxDiameterPairs;

    for (int i = 0; i < getNumVertex(); ++i) {
        bfsPath(i, Airline::AirlineH()); // Run BFS from each vertex

        vector<vector<int>> paths;
        vector<int> path;
        findPaths(paths, path, i); // Find all paths from the current vertex

        for (const auto& p : paths) {
            if (vertexSet[p.front()]->distance == d) {
                string source = vertexSet[i]->getAirport().getCode();
                string destination = vertexSet[p.back()]->getAirport().getCode();
                maxDiameterPairs.emplace_back(source, destination);
            }
        }
    }

    return maxDiameterPairs;
}

//TODO check for all cases
void Graph::dfsArt(int v, int index, list<int>& res, Airline::AirlineH airlines) {
    vertexSet[v]->num = vertexSet[v]->low = index++;
    vertexSet[v]->art = true;
    int count = 0;

    for (const auto& e : vertexSet[v]->getAdj()) {
        auto w = e.dest->getId();

        if (airlines.find(e.airline) != airlines.end() || airlines.empty()) {
            if (vertexSet[w]->num == 0) {
                count++;
                dfsArt(w, index, res, airlines);
                vertexSet[v]->low = min(vertexSet[v]->low, vertexSet[w]->low);

                if (vertexSet[w]->low >= vertexSet[v]->num && std::find(res.begin(),res.end(),v) == res.end()) {
                    if(index == 1 && count > 1)
                        res.push_back(1);
                    else if (index != 2 && std::find(res.begin(),res.end(),v) == res.end())
                        res.push_back(v);
                }
            } else if (vertexSet[v]->art) { // Ignore edge to the parent in DFS tree
                vertexSet[v]->low = min(vertexSet[v]->low, vertexSet[w]->num);
            }
        }
    }
}

list<int> Graph::articulationPoints(const Airline::AirlineH& airlines) {
    list<int> res;

    for(int i = 0; i < size; i++){
        vertexSet[i]->setVisited(false);
        vertexSet[i]->art = false;
        vertexSet[i]->num = 0;
        vertexSet[i]->low = {};
    }

    int index = 1;

    for(int i = 0; i < size; i++)
        if (vertexSet[i]->num == 0)
            dfsArt(i, index,res, airlines);

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