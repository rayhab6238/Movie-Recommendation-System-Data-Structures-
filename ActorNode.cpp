#include "ActorNode.h"
#include "MovieNode.h"
#include <iostream>
using namespace std;

ActorNode::ActorNode(const string& actorName) : name(actorName), head(nullptr) {}

ActorNode::~ActorNode() {
    MovieLinkedlistNode* cur = head;
    while (cur) { MovieLinkedlistNode* nx = cur->next; delete cur; cur = nx; }
    head = nullptr;
}

void ActorNode::addMovie(MovieNode* m) {
    if (!m) return;
    MovieLinkedlistNode* cur = head;
    while (cur) { if (cur->movie == m) return; cur = cur->next; }
    MovieLinkedlistNode* n = new MovieLinkedlistNode(m); n->next = head; head = n;
}

void ActorNode::removeMovie(MovieNode* m) {
    MovieLinkedlistNode* cur = head; MovieLinkedlistNode* prev = nullptr;
    while (cur) {
        if (cur->movie == m) { if (!prev) head = cur->next; else prev->next = cur->next; delete cur; return; }
        prev = cur; cur = cur->next;
    }
}

void ActorNode::replaceMoviePointer(MovieNode* oldM, MovieNode* newM) {
    MovieLinkedlistNode* cur = head;
    while (cur) { if (cur->movie == oldM) cur->movie = newM; cur = cur->next; }
}

void ActorNode::printFilmography() {
    if (!head) { cout << "No movies found for actor: " << name << "\n"; return; }
    cout << "Filmography of " << name << ":\n";
    MovieLinkedlistNode* cur = head;
    while (cur) { cout << " - " << cur->movie->title << " (" << cur->movie->year << "), Rating: " << cur->movie->score << "\n"; cur = cur->next; }
}

void ActorNode::printCoActors() {
    if (!head) { cout << "No movies found for actor: " << name << "\n"; return; }
    int cap = 16, sz = 0;
    string* arr = new string[cap];
    MovieLinkedlistNode* cur = head;
    while (cur) {
        MovieNode* m = cur->movie;
        string candidates[3] = { m->actor1, m->actor2, m->actor3 };
        for (int i = 0; i < 3; i++) {
            string& cand = candidates[i]; if (cand.empty() || caseInsensitiveEqual(cand, name)) continue;
            bool found = false; for (int j = 0; j < sz; j++) if (caseInsensitiveEqual(arr[j], cand)) found = true;
            if (!found) {
                if (sz >= cap) { int newcap = cap * 2; string* tmp = new string[newcap]; for (int t = 0; t < sz; t++) tmp[t] = arr[t]; delete[] arr; arr = tmp; cap = newcap; }
                arr[sz++] = cand;
            }
        }
        cur = cur->next;
    }
    if (sz == 0) cout << "No co-actors found for " << name << "\n";
    else { cout << "Co-actors of " << name << ":\n"; for (int i = 0; i < sz; i++) cout << " - " << arr[i] << "\n"; }
    delete[] arr;
}

