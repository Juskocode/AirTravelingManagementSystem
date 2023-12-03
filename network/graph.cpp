#include "graph.h"


template<class T>
Graph<T>::Graph(int Vertexs) {
    vertexSet.resize(Vertexs);
}

template <class T>
int Graph::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return visited;
}

template <class T>
void Vertex<T>::setVisited(bool v) {
    Vertex::visited = v;
}

template<class T>
const vector<Edge<T>> &Vertex<T>::getAdj() const {
    return adj;
}

template <class T>
void Vertex<T>::setAdj(const vector<Edge<T>> &adj) {
    Vertex::adj = adj;
}


/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &src){
    if ( findVertex(src) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(src));
    return true;
}


/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, const Airline &airline, double w) {
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
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
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
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
        if ((*it)->info  == in) {
            auto v = *it;
            vertexSet.erase(it);
            for (auto u : vertexSet)
                u->removeEdgeTo(v);
            delete v;
            return true;
        }
    return false;
}

template<class T>
bool Graph<T>::addAirport(const T &source, const Airport &airport) {

    auto v = findVertex(source);

    if ( findVertex(source) != NULL)
        return false;

    v->airport = airport;
}

template<class T>
double Graph<T>::distance(double lat1, double lon1, double lat2, double lon2) {
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
template <class T>
vector<T> Graph<T>::dfs() const {
    vector<T> res;
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
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
    v->setVisited(true); // Marking the current vertex as visited
    res.push_back(v->getInfo()); // Adding current vertex info to the result vector

    for (auto edge : v->getAdj()) {
        Vertex<T> *adjVertex = edge.getDest();
        if (!adjVertex->isVisited()) {
            dfsVisit(adjVertex, res); // Recursive call for unvisited adjacent vertices
        }
    }
}


// TODO
template <class T>
vector<T> Graph<T>::dfs(const T & source) const {
    Vertex<T>* sourceVertex = findVertex(source);
    if (sourceVertex == nullptr)
        return {};

    vector<T> res;
    for (auto v : vertexSet) {
        v->setVisited(false); // Resetting visited flag for all vertices
    }

    // Initiating DFS traversal from the source node
    dfsVisit(sourceVertex, res);

    return res;
}


// TODO
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {

    auto src = findVertex(source);

    if(src == NULL)
        return {};

    for (int i = 1; i <= getNumVertex(); i++) {
        vertexSet[i]->setVisited(false);
        vertexSet[i]->distance = 0;
    }

    vector<T> res;

    queue<Vertex<T>*> q;
    q.push(src);

    vertexSet[src]->visited = true;

    while(!q.empty()){

        Vertex<T>* currV = q.front();q.pop();
        res.push_back(currV->getInfo());

        for(const Edge<T> &e : currV->adj){

            Vertex<T>* dst = e.getDest();

            if(!vertexSet[dst]->isVisited()){
                q.push(dst);
                dst->setVisited(true);
            }
        }
    }

    return res;
}