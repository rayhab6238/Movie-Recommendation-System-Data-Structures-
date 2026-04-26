#ifndef MOVIENODE_H
#define MOVIENODE_H

#include "utils.h"

class ActorNode;

class MovieNode {
public:
    string director, duration, actor1, actor2, actor3, genres, title, budget, year, score;
    MovieNode* left;
    MovieNode* right;
    int height;
    GenreLinkedlistNode* genreHead;
    ActorLinkedlistNode* actorHead;

    MovieNode(string d_director, string d_duration, string d_actor2, string d_genres,
        string d_actor1, string d_title, string d_actor3, string d_budget,
        string d_year, string d_score);

    void addActorRef(ActorNode* a);
    void removeActorRef(ActorNode* a);
    void printActors();
    void addGenre(const string& g);
    void populateGenres(const string& gstring);

    ~MovieNode();
};

#endif

