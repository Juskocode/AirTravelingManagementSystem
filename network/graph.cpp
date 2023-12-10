#include "graph.h"
#include "../classes/Parser.h"

Graph::Graph(int Vertexs) {
    vertexSet.resize(Vertexs);
}


int Graph::getNumVertex() const {
    return vertexSet.size();
}

vector<Vertex*> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex * Graph::findVertex(const int &in) const {
    for (auto v : vertexSet)
        if (v->getId() == in)
            return v;
    return nullptr;
}

bool Vertex::isVisited() const {
    return visited;
}

void Vertex::setVisited(bool v) {
    Vertex::visited = v;
}

const vector<Edge> &Vertex::getAdj() const {
    return adj;
}

void Vertex::setAdj(const vector<Edge> &adj) {
    Vertex::adj = adj;
}

double Vertex::getDistance(){
    return distance;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &src){
    if ( findVertex(src) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(src));
    return true;
}


/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &sourc, const int &dest, const Airline &airline, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, airline, w);
    return true;
}




/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
bool Graph::removeEdge(const int &sourc, const int &dest) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    return v1->removeEdgeTo(v2);
}



/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
bool Graph::removeVertex(const int &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
        if ((*it)->getId()  == in) {
            auto v = *it;
            vertexSet.erase(it);
            for (auto u : vertexSet)
                u->removeEdgeTo(v);
            delete v;
            return true;
        }
    return false;
}

bool Graph::addAirport(const int &source, const Airport &airport) {

    auto v = findVertex(source);

    if ( findVertex(source) != NULL)
        return false;

    v->airport = airport;
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


// TODO
vector<int> Graph::dfs() const {
    vector<int> res;
    for (auto v : vertexSet) {
        v->setVisited(false); // Resetting visited flag for all vertices
    }

    for (auto v : vertexSet) {
        if (!v->isVisited()) {
            dfsVisit(v, res);
        }
    }

    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Updates a parameter with the list of visited node contents.
 */
// TODO
void Graph::dfsVisit(Vertex *v, vector<int> & res) const {
    v->setVisited(true); // Marking the current vertex as visited
    res.push_back(v->getId()); // Adding current vertex info to the result vector

    for (const auto& edge : v->getAdj()) {
        Vertex *adjVertex = edge.getDest();
        if (!adjVertex->isVisited()) {
            dfsVisit(adjVertex, res); // Recursive call for unvisited adjacent vertices
        }
    }
}


// TODO
vector<int> Graph::dfs(const int& source) const {
    Vertex* sourceVertex = findVertex(source);
    if (sourceVertex == nullptr)
        return {};

    vector<int> res;
    for (auto v : vertexSet) {
        v->setVisited(false); // Resetting visited flag for all vertices
    }

    // Initiating DFS traversal from the source node
    dfsVisit(sourceVertex, res);

    return res;
}


// TODO
vector<int> Graph::bfs(const  int& source) const {

    auto src = findVertex(source);

    if(src == nullptr)
        return {};

    for (int i = 1; i <= getNumVertex(); i++) {
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = 0;
    }

    vector<int> res;

    queue<Vertex*> q;
    q.push(src);

    vertexSet[src->getId()]->visited = true;

    while(!q.empty()){

        Vertex* currV = q.front();q.pop();
        res.push_back(currV->getId());

        for(const Edge &e : currV->adj){

            Vertex* dst = e.getDest();

            if(!vertexSet[dst->getId()]->isVisited()){
                q.push(dst);
                dst->setVisited(true);
            }
        }
    }

    return res;
}

int Graph::nrFlights(int src, int dest, Airline::AirlineH airlines){

    auto source = findVertex(src);
    auto destination = findVertex(dest);


    if(source == nullptr || destination == nullptr)
        return {};

    for (int i = 1; i <= getNumVertex(); i++) {
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = 0;
    }

    queue<Vertex*> q;
    q.push(source);

    vertexSet[source->getId()]->visited = true;

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

    return (int)destination->distance;
}


void Graph::bfsPath(int src, Airline::AirlineH airlines){
    auto source = findVertex(src);

    if(source == nullptr)
        return;

    for (int i = 1; i <= getNumVertex(); i++) {
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->parents.clear();
    }

    queue<Vertex*> q;
    q.push(source);

    vertexSet[source->getId()]->parents = {-1};
    vertexSet[source->getId()]->distance = 0;

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

    auto V = findVertex(v);

    if (V == nullptr) {
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
    auto source = findVertex(src);
    auto destination = findVertex(dest);


    if(source == nullptr || destination == nullptr)
        return {};

    //node id and node value(distance)
    MinHeap<int, double> minHeap(getNumVertex(), -1);

    for(int i = 1; i <= getNumVertex(); i++){
        vertexSet[i]->distance = INT_MAX;
        vertexSet[i]->setVisited(false);
        vertexSet[i]->parents.clear();
        minHeap.insert(i, INT_MAX);
    }

    vertexSet[source->getId()]->distance = 0;
    vertexSet[source->getId()]->parents.push_back(source->getId());

    minHeap.decreaseKey(source->getId(), 0);

    while(!minHeap.empty()){

        auto u = minHeap.extractMin();
        vertexSet[source->getId()]->setVisited(true);

        for(const auto &e : vertexSet[u]->getAdj()){

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            auto v = e.getDest()->getId();
            double w = e.getWeight();

            if(vertexSet[v]->isVisited() && vertexSet[u]->distance + w < vertexSet[v]->distance){

                vertexSet[v]->distance = vertexSet[u]->distance + w;

                auto p = vertexSet[u]->parents;
                if (find(p.begin(), p.end(), v) == p.end()) p.push_back(v);

                vertexSet[v]->parents = p;
                minHeap.decreaseKey(v, vertexSet[v]->distance);

            }
        }
    }

    return vertexSet[destination->getId()];
}


vector<string> Graph::getAirlines(int src, int dest, Airline::AirlineH airlines) {
    vector<string> usedAirlines;
    for (const auto& e: vertexSet[src]->adj)
        if (e.dest->getId() == dest && (airlines.empty() || airlines.find(e.airline) != airlines.end()))
            usedAirlines.push_back(e.airline.getCode());
    return usedAirlines;
}


void Graph::printPath(vector<int> path, const Airline::AirlineH& airlines) {
    for (int i = 0; i < path.size()-1; i++){
        auto possibleAirlines = getAirlines(path[i],path[i+1],airlines);
        printf("\033[1m\033[46m %s \033[0m", vertexSet[path[i]]->airport.getCode().c_str());
        cout <<" --- (";
        for (int j = 0; j < possibleAirlines.size()-1; j++)
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
