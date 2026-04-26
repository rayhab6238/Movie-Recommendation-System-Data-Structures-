#include "MovieGraph.h"
#include "utils.h"
#include <iostream>
using namespace std;

#include "MovieGraph.h"
#include "utils.h"
#include <iostream>
using namespace std;

// -------------------- GraphVertex Constructor --------------------
// Initialize a vertex with a pointer to a MovieNode and an empty adjacency list
GraphVertex::GraphVertex(MovieNode* m) : movie(m), adjacencyList(nullptr) {}

// -------------------- GraphVertex Destructor --------------------
// Delete all edges linked to this vertex to avoid memory leaks
GraphVertex::~GraphVertex() {
    GraphEdge* cur = adjacencyList;
    while (cur) {
        GraphEdge* nx = cur->next; // store next edge before deletion
        delete cur;                // delete current edge
        cur = nx;                  // move to next
    }
}

// -------------------- Add Edge to Vertex --------------------
// Adds a connection from this movie to another movie
void GraphVertex::addEdge(MovieNode* other, string type, string entity) {
    if (!other || other == movie) return; // skip if null or same movie

    // Check if the connection already exists to prevent duplicates
    GraphEdge* cur = adjacencyList;
    while (cur) {
        if (cur->movie == other) return; // edge exists
        cur = cur->next;
    }

    // Insert new edge at the front of adjacency list
    GraphEdge* edge = new GraphEdge(other, type, entity);
    edge->next = adjacencyList;
    adjacencyList = edge;
}

// -------------------- Print Connections --------------------
// Print all movies connected to this vertex and their shared attribute
void GraphVertex::printConnections() {
    if (!adjacencyList) {
        cout << "No connections found.\n";
        return;
    }

    cout << "\nConnections for \"" << movie->title << "\":\n";
    cout << "---------------------------------------\n";

    GraphEdge* cur = adjacencyList;
    int count = 0;
    while (cur) {
        count++;
        cout << count << ". " << cur->movie->title << " (" << cur->movie->year << ")\n";
        cout << "   Connection: " << cur->connectionType << " - " << cur->sharedEntity << "\n";
        cur = cur->next; // move to next connection
    }
    cout << "\nTotal connections: " << count << "\n";
}

// -------------------- Get Connection Count --------------------
// Return how many movies this movie is connected to
int GraphVertex::getConnectionCount() {
    int count = 0;
    GraphEdge* cur = adjacencyList;
    while (cur) {
        count++;
        cur = cur->next;
    }
    return count;
}

// -------------------- Hash Function --------------------
// Converts a movie title to a hash table index (case-insensitive)
int MovieGraph::hashTitle(const string& title) {
    unsigned long hash = 5381; // starting prime number
    for (int i = 0; i < (int)title.size(); i++) {
        char c = title[i];
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a'; // convert uppercase to lowercase
        hash = ((hash << 5) + hash) + (unsigned long)c; // hash * 33 + c
    }
    return hash % capacity; // index within table size
}

// -------------------- Resize Hash Table --------------------
// Double the table size and rehash all existing vertices
void MovieGraph::resize() {
    int oldCapacity = capacity;
    GraphVertex** oldVertices = vertices; // save old table

    capacity *= 2; // new capacity is double
    vertices = new GraphVertex * [capacity];
    for (int i = 0; i < capacity; i++) vertices[i] = nullptr;

    count = 0; // reset count

    // Re-insert all old vertices into new table
    for (int i = 0; i < oldCapacity; i++) {
        if (oldVertices[i]) {
            addVertex(oldVertices[i]->movie); // rehash into new table
            delete oldVertices[i];            // free old vertex
        }
    }
    delete[] oldVertices; // delete old array
}

// -------------------- MovieGraph Constructor --------------------
// Initialize a MovieGraph with given capacity
MovieGraph::MovieGraph(int cap) {
    capacity = cap;
    count = 0;
    vertices = new GraphVertex * [capacity];
    for (int i = 0; i < capacity; i++) vertices[i] = nullptr; // empty table
}

// -------------------- MovieGraph Destructor --------------------
// Delete all vertices to avoid memory leaks
MovieGraph::~MovieGraph() {
    for (int i = 0; i < capacity; i++) {
        if (vertices[i]) delete vertices[i];
    }
    delete[] vertices;
}

// -------------------- Add Vertex --------------------
// Add a movie to the graph
void MovieGraph::addVertex(MovieNode* movie) {
    if (!movie) return; // ignore null movies

    // Resize table if load factor exceeds 0.7
    if ((float)count / capacity > 0.7) resize();

    int idx = hashTitle(movie->title); // get hash index
    int probes = 0;

    // Handle collisions using linear probing
    while (vertices[idx] != nullptr && probes < capacity) {
        if (caseInsensitiveEqual(vertices[idx]->movie->title, movie->title)) {
            return; // movie already exists
        }
        idx = (idx + 1) % capacity;
        probes++;
    }

    if (probes < capacity) { // insert if space found
        vertices[idx] = new GraphVertex(movie);
        count++;
    }
}

// -------------------- Find Vertex --------------------
// Search for a movie vertex by title (case-insensitive)
GraphVertex* MovieGraph::findVertex(const string& title) {
    int idx = hashTitle(title); // compute hash index
    int probes = 0;

    while (vertices[idx] != nullptr && probes < capacity) {
        if (caseInsensitiveEqual(vertices[idx]->movie->title, title)) {
            return vertices[idx]; // found
        }
        idx = (idx + 1) % capacity; // linear probe
        probes++;
    }
    return nullptr; // not found
}

// -------------------- Add Edge Between Two Movies --------------------
// Connect two movies in the graph with a shared attribute
void MovieGraph::addEdge(MovieNode* movie1, MovieNode* movie2, string type, string entity) {
    if (!movie1 || !movie2) return; // ignore if either movie is null

    GraphVertex* v1 = findVertex(movie1->title); // find vertex for first movie
    GraphVertex* v2 = findVertex(movie2->title); // find vertex for second movie

    if (v1 && v2) {
        // Add edges in both directions since the graph is undirected
        v1->addEdge(movie2, type, entity);
        v2->addEdge(movie1, type, entity);
    }
}

// -------------------- Print Connections for a Movie --------------------
void MovieGraph::printMovieConnections(const string& title) {
    GraphVertex* v = findVertex(title); // find vertex
    if (v) {
        v->printConnections(); // print all neighbors
    }
    else {
        cout << "Movie not found in graph: " << title << "\n";
    }
}

// -------------------- Display Graph Statistics --------------------
// Useful to see total vertices, edges, and most connected movie
void MovieGraph::displayStatistics() {
    cout << "\n========== GRAPH STATISTICS ==========\n";
    cout << "Total vertices (movies): " << count << "\n";

    int totalEdges = 0;
    int maxConnections = 0;
    string mostConnectedMovie;

    // Traverse all vertices to calculate stats
    for (int i = 0; i < capacity; i++) {
        if (vertices[i]) {
            int connections = vertices[i]->getConnectionCount();
            totalEdges += connections;

            if (connections > maxConnections) {
                maxConnections = connections;
                mostConnectedMovie = vertices[i]->movie->title;
            }
        }
    }

    // Divide by 2 because edges are stored twice in undirected graph
    cout << "Total edges: " << (totalEdges / 2) << " (undirected)\n";
    cout << "Average connections per movie: " << (count > 0 ? (float)totalEdges / count : 0) << "\n";
    cout << "Most connected movie: " << mostConnectedMovie << " (" << maxConnections << " connections)\n";
}

// -------------------- Recommend Movies Using BFS --------------------
// Recommends movies based on proximity in the graph 
void MovieGraph::recommendMovies(const string& title, int maxRecommendations) {
    GraphVertex* start = findVertex(title); // starting movie

    if (!start) {
        cout << "Movie not found in graph: " << title << "\n";
        return;
    }

    cout << "\n========== RECOMMENDATIONS FOR: " << title << " ==========\n";

    // Track visited vertices to avoid revisiting
    bool* visited = new bool[capacity];
    for (int i = 0; i < capacity; i++) visited[i] = false;

    // Mark the start vertex as visited using linear probing
    int startIdx = hashTitle(start->movie->title);
    int probe = 0;
    while (vertices[startIdx] && vertices[startIdx] != start && probe < capacity) {
        startIdx = (startIdx + 1) % capacity;
        probe++;
    }
    if (probe < capacity) visited[startIdx] = true;

    // -------------------- Queue Node for BFS --------------------
    struct QueueNode {
        GraphVertex* vertex;
        int distance;          // distance from starting movie
        string connectionPath; // stores connection type
        QueueNode* next;
        QueueNode(GraphVertex* v, int d, string path)
            : vertex(v), distance(d), connectionPath(path), next(nullptr) {
        }
    };

    QueueNode* queueHead = new QueueNode(start, 0, "");
    QueueNode* queueTail = queueHead;

    // -------------------- Recommendation Structure --------------------
    struct Recommendation {
        MovieNode* movie;
        int distance;
        string connections;
        float score;
    };

    Recommendation* recommendations = new Recommendation[maxRecommendations * 3];
    int recCount = 0;

    // -------------------- BFS Traversal --------------------
    while (queueHead && recCount < maxRecommendations * 3) {
        QueueNode* current = queueHead;
        queueHead = queueHead->next;
        if (!queueHead) queueTail = nullptr;

        // Explore all neighbors
        GraphEdge* edge = current->vertex->adjacencyList;
        while (edge && recCount < maxRecommendations * 3) {
            // Find the neighbor's index using hash table
            int idx = hashTitle(edge->movie->title);
            probe = 0;
            while (vertices[idx] && vertices[idx]->movie != edge->movie && probe < capacity) {
                idx = (idx + 1) % capacity;
                probe++;
            }

            if (probe < capacity && vertices[idx] && !visited[idx]) {
                visited[idx] = true; // mark as visited
                GraphVertex* nextVertex = vertices[idx];
                int newDist = current->distance + 1;

                // Only consider movies within distance 5
                if (newDist <= 5) {
                    // Add to BFS queue
                    QueueNode* newNode = new QueueNode(nextVertex, newDist,
                        edge->connectionType + ":" + edge->sharedEntity);

                    if (queueTail) {
                        queueTail->next = newNode;
                        queueTail = newNode;
                    }
                    else {
                        queueHead = queueTail = newNode;
                    }

                    // Add to recommendation list (exclude start movie)
                    if (newDist > 0) {
                        recommendations[recCount].movie = nextVertex->movie;
                        recommendations[recCount].distance = newDist;
                        recommendations[recCount].connections = newNode->connectionPath;

                        

                        recCount++;
                    }
                }
            }

            edge = edge->next;
        }

        delete current; // free current node
    }

    // Clean up remaining queue nodes
    while (queueHead) {
        QueueNode* temp = queueHead;
        queueHead = queueHead->next;
        delete temp;
    }
    delete[] visited;

    if (recCount == 0) {
        cout << "No recommendations found. Movie may not be connected in the graph.\n";
        delete[] recommendations;
        return;
    }

    // -------------------- Sort Recommendations by Score --------------------
    // Simple bubble sort (can be optimized later)
    for (int i = 0; i < recCount - 1; i++) {
        for (int j = 0; j < recCount - i - 1; j++) {
            if (recommendations[j].score < recommendations[j + 1].score) {
                Recommendation temp = recommendations[j];
                recommendations[j] = recommendations[j + 1];
                recommendations[j + 1] = temp;
            }
        }
    }

    // -------------------- Display Top Recommendations --------------------
    int displayCount = recCount < maxRecommendations ? recCount : maxRecommendations;
    cout << "\nTop " << displayCount << " Recommendations:\n";
    cout << "---------------------------------------\n";

    for (int i = 0; i < displayCount; i++) {
        cout << (i + 1) << ". " << recommendations[i].movie->title
            << " (" << recommendations[i].movie->year << ")\n";
        cout << "   Rating: " << recommendations[i].movie->score
            << " | Distance: " << recommendations[i].distance << " hop(s)\n";
        cout << "   Connection: " << recommendations[i].connections << "\n";
        cout << "   Genres: " << recommendations[i].movie->genres << "\n\n";
    }

    delete[] recommendations; // free memory
}

void MovieGraph::findShortestPath(const string& movie1Title, const string& movie2Title) {
    GraphVertex* start = findVertex(movie1Title);
    GraphVertex* end = findVertex(movie2Title);

    if (!start) {
        cout << "Movie not found in graph: " << movie1Title << "\n";
        return;
    }
    if (!end) {
        cout << "Movie not found in graph: " << movie2Title << "\n";
        return;
    }

    if (caseInsensitiveEqual(movie1Title, movie2Title)) {
        cout << "Both movies are the same!\n";
        return;
    }

    cout << "\n========== SHORTEST PATH ==========\n";
    cout << "From: " << movie1Title << "\n";
    cout << "To:   " << movie2Title << "\n";
    cout << "-----------------------------------\n";

    // BFS with parent tracking
    struct PathInfo {
        int parentIndex;
        string connectionType;
        string sharedEntity;
        MovieNode* movie;
    };

    PathInfo* pathInfo = new PathInfo[capacity];
    bool* visited = new bool[capacity];

    for (int i = 0; i < capacity; i++) {
        visited[i] = false;
        pathInfo[i].parentIndex = -1;
        pathInfo[i].movie = nullptr;
    }

    // Find start index
    int startIdx = -1;
    for (int i = 0; i < capacity; i++) {
        if (vertices[i] && vertices[i] == start) {
            startIdx = i;
            break;
        }
    }

    if (startIdx == -1) {
        cout << "Error: Could not locate start movie.\n";
        delete[] visited;
        delete[] pathInfo;
        return;
    }

    visited[startIdx] = true;
    pathInfo[startIdx].movie = start->movie;

    // Queue for BFS
    struct QueueNode {
        int vertexIndex;
        QueueNode* next;
        QueueNode(int idx) : vertexIndex(idx), next(nullptr) {}
    };

    QueueNode* queueHead = new QueueNode(startIdx);
    QueueNode* queueTail = queueHead;

    bool found = false;
    int endIdx = -1;

    // BFS traversal
    while (queueHead && !found) {
        QueueNode* current = queueHead;
        queueHead = queueHead->next;
        if (!queueHead) queueTail = nullptr;

        int currentIdx = current->vertexIndex;
        GraphVertex* currentVertex = vertices[currentIdx];

        // Check if we reached the destination
        if (currentVertex == end) {
            found = true;
            endIdx = currentIdx;
            delete current;
            break;
        }

        // Explore neighbors
        GraphEdge* edge = currentVertex->adjacencyList;
        while (edge) {
            // Find the index of this neighbor
            int neighborIdx = -1;
            for (int i = 0; i < capacity; i++) {
                if (vertices[i] && vertices[i]->movie == edge->movie) {
                    neighborIdx = i;
                    break;
                }
            }

            if (neighborIdx != -1 && !visited[neighborIdx]) {
                visited[neighborIdx] = true;
                pathInfo[neighborIdx].parentIndex = currentIdx;
                pathInfo[neighborIdx].connectionType = edge->connectionType;
                pathInfo[neighborIdx].sharedEntity = edge->sharedEntity;
                pathInfo[neighborIdx].movie = edge->movie;

                // Add to queue
                QueueNode* newNode = new QueueNode(neighborIdx);
                if (queueTail) {
                    queueTail->next = newNode;
                    queueTail = newNode;
                }
                else {
                    queueHead = queueTail = newNode;
                }

                // Check if this is the destination
                if (edge->movie == end->movie) {
                    found = true;
                    endIdx = neighborIdx;
                    delete current;
                    // Clean up queue
                    while (queueHead) {
                        QueueNode* temp = queueHead;
                        queueHead = queueHead->next;
                        delete temp;
                    }
                    goto path_found;
                }
            }

            edge = edge->next;
        }

        delete current;
    }

path_found:
    // Clean up any remaining queue nodes
    while (queueHead) {
        QueueNode* temp = queueHead;
        queueHead = queueHead->next;
        delete temp;
    }

    if (!found) {
        cout << "No path found between these two movies.\n";
        cout << "They may not be connected in the graph.\n";
        delete[] visited;
        delete[] pathInfo;
        return;
    }

    // Reconstruct path from end to start
    struct PathNode {
        MovieNode* movie;
        string connectionType;
        string sharedEntity;
        PathNode* next;
        PathNode(MovieNode* m) : movie(m), next(nullptr) {}
    };

    PathNode* pathHead = nullptr;
    int currentIdx = endIdx;
    int pathLength = 0;

    while (currentIdx != -1) {
        PathNode* node = new PathNode(pathInfo[currentIdx].movie);
        if (pathInfo[currentIdx].parentIndex != -1) {
            node->connectionType = pathInfo[currentIdx].connectionType;
            node->sharedEntity = pathInfo[currentIdx].sharedEntity;
        }
        node->next = pathHead;
        pathHead = node;
        currentIdx = pathInfo[currentIdx].parentIndex;
        pathLength++;
    }

    delete[] visited;
    delete[] pathInfo;

    // Display the path
    cout << "\nPath found! Length: " << (pathLength - 1) << " hop(s)\n";
    cout << "---------------------------------------\n";

    PathNode* current = pathHead;
    int step = 0;
    while (current) {
        if (step == 0) {
            cout << "START: " << current->movie->title
                << " (" << current->movie->year << ")\n";
        }
        else {
            cout << "\n  ? [via " << current->connectionType << ": "
                << current->sharedEntity << "]\n\n";
            cout << "Step " << step << ": " << current->movie->title
                << " (" << current->movie->year << ")\n";
            cout << "        Rating: " << current->movie->score
                << " | Genre: " << current->movie->genres << "\n";
        }

        PathNode* temp = current;
        current = current->next;
        delete temp;
        step++;
    }

    cout << "\n---------------------------------------\n";
    cout << "Connection established!\n";
}

