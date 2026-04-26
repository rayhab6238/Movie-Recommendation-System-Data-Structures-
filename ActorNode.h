#ifndef ACTORNODE_H
#define ACTORNODE_H

#include "utils.h"

class MovieNode;

class ActorNode {
public:
    string name;
    struct MovieLinkedlistNode {
        MovieNode* movie;
        MovieLinkedlistNode* next;
        MovieLinkedlistNode(MovieNode* m) : movie(m), next(nullptr) {}
    };
    MovieLinkedlistNode* head;

    ActorNode(const string& actorName);
    ~ActorNode();

    void addMovie(MovieNode* m);
    void removeMovie(MovieNode* m);
    void replaceMoviePointer(MovieNode* oldM, MovieNode* newM);
    void printFilmography();
    void printCoActors();
};

#endif

