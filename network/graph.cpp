#include "graph.h"


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
    return NULL;
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


/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &src){
    if ( findVertex(src) != NULL)
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
    if (v1 == NULL || v2 == NULL)
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
    if (v1 == NULL || v2 == NULL)
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

    for (auto edge : v->getAdj()) {
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

    if(src == NULL)
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