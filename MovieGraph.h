#pragma once
#ifndef MOVIEGRAPH_H
#define MOVIEGRAPH_H

#include "utils.h"
#include "MovieNode.h"
#include <iostream>
using namespace std;

// Edge represents a connection between two movies
struct GraphEdge {
    MovieNode* movie;
    string connectionType; // "actor" or "genre"
    string sharedEntity;   // name of shared actor or genre
    GraphEdge* next;

    GraphEdge(MovieNode* m, string type, string entity)
        : movie(m), connectionType(type), sharedEntity(entity), next(nullptr) {
    }
};

// Graph vertex wraps a movie and its adjacency list
class GraphVertex {
public:
    MovieNode* movie;
    GraphEdge* adjacencyList;

    GraphVertex(MovieNode* m);
    ~GraphVertex();

    void addEdge(MovieNode* other, string type, string entity);
    void printConnections();
    int getConnectionCount();
};

// Movie Graph manages all vertices and provides graph operations
class MovieGraph {
private:
    GraphVertex** vertices;
    int capacity;
    int count;

    int hashTitle(const string& title);
    void resize();

public:
    MovieGraph(int cap = 4096);
    ~MovieGraph();

    void addVertex(MovieNode* movie);
    GraphVertex* findVertex(const string& title);
    void addEdge(MovieNode* movie1, MovieNode* movie2, string type, string entity);
    void printMovieConnections(const string& title);
    void displayStatistics();
    void recommendMovies(const string& title, int maxRecommendations = 10);
    void findShortestPath(const string& movie1Title, const string& movie2Title);
};

#endif

