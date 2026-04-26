#ifndef MOVIEMANAGER_H
#define MOVIEMANAGER_H

#include "utils.h"
#include "MovieNode.h"
#include "ActorHashTable.h"
#include "MovieGraph.h"

class MovieManager {
private:
    MovieNode* root;
    ActorHashTable actorTable;
    MovieGraph* graph;

    int height(MovieNode* n);
    int getBalance(MovieNode* n);
    void addVerticesToGraph(MovieNode* node);
    void buildEdgesUsingActorIndex();
    void buildGenreEdges(MovieNode* node);
    void findMoviesWithSharedGenres(MovieNode* compareNode, MovieNode* targetNode);
    MovieNode* rightRotate(MovieNode* y);
    MovieNode* leftRotate(MovieNode* x);
    MovieNode* insert(MovieNode* node, MovieNode* newNode);
    void inOrder(MovieNode* node);
    void searchByTitle(MovieNode* node, string title);
    void searchByActorScan(MovieNode* node, const string& actor);
    void searchByGenre(MovieNode* node, string genre);
    void searchByYear(MovieNode* node, string year);
    void searchByRating(MovieNode* node, float low, float high);
    MovieNode* minValueNode(MovieNode* node);
    MovieNode* findNodeByTitle(MovieNode* node, const string& title);
    void removeMovieFromActors(MovieNode* m);
    MovieNode* deleteNode(MovieNode* root, string title);
    void destroyTree(MovieNode* node);

public:
    MovieManager();
    void buildGraph();
    void printMovieConnections(const string& title);
    void displayGraphStats();
    void addMovie(MovieNode* m);
    void display();
    void searchByTitle(const string& title);
    void searchByActor(const string& actor);
    void searchByGenre(const string& genre);
    void searchByYear(const string& year);
    void searchByRating(float low, float high);
    void deleteMovie(const string& title);
    void printActorFilmography(const string& actor);
    void printActorCoActors(const string& actor);
    void recommendMovies(const string& title, int count = 10);
    void findShortestPath(const string& movie1, const string& movie2);
};

#endif

