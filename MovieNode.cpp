#include "MovieNode.h"
#include "ActorNode.h"

MovieNode::MovieNode(string d_director, string d_duration, string d_actor2, string d_genres,
    string d_actor1, string d_title, string d_actor3, string d_budget,
    string d_year, string d_score)
{
    director = d_director;
    duration = d_duration;
    actor2 = d_actor2;
    genres = d_genres;
    actor1 = d_actor1;
    title = d_title;
    actor3 = d_actor3;
    budget = d_budget;
    year = d_year;
    score = d_score;

    left = right = nullptr;
    height = 1;
    genreHead = nullptr;
    actorHead = nullptr;
}

MovieNode::~MovieNode() {
    GenreLinkedlistNode* gcur = genreHead;
    while (gcur) { GenreLinkedlistNode* nx = gcur->next; delete gcur; gcur = nx; }
    genreHead = nullptr;

    ActorLinkedlistNode* acur = actorHead;
    while (acur) { ActorLinkedlistNode* nx = acur->next; delete acur; acur = nx; }
    actorHead = nullptr;
}

void MovieNode::addActorRef(ActorNode* a) {
    if (!a) return;
    ActorLinkedlistNode* cur = actorHead;
    while (cur) { 
        if (cur->actor && caseInsensitiveEqual(cur->actor->name, a->name)) return; 
        cur = cur->next; 
    }
    ActorLinkedlistNode* n = new ActorLinkedlistNode(a); n->next = actorHead; actorHead = n;
}

void MovieNode::removeActorRef(ActorNode* a) {
    if (!a || !actorHead) return;
    ActorLinkedlistNode* cur = actorHead;
    ActorLinkedlistNode* prev = nullptr;
    while (cur) {
        if (cur->actor && caseInsensitiveEqual(cur->actor->name, a->name)) {
            if (!prev) actorHead = cur->next;
            else prev->next = cur->next;
            delete cur;
            return;
        }
        prev = cur; cur = cur->next;
    }
}

void MovieNode::printActors() {
    if (!actorHead) {
        cout << "Actors: " << actor1;
        if (!actor2.empty()) cout << ", " << actor2;
        if (!actor3.empty()) cout << ", " << actor3;
        cout << "\n"; return;
    }
    cout << "Actors: ";
    ActorLinkedlistNode* cur = actorHead;
    bool first = true;
    while (cur) {
        if (!first) cout << ", ";
        cout << cur->actor->name;
        first = false; cur = cur->next;
    }
    cout << "\n";
}

void MovieNode::addGenre(const string& g) {
    if (g.empty()) return;
    GenreLinkedlistNode* cur = genreHead;
    while (cur) { if (caseInsensitiveEqual(cur->genre, g)) return; cur = cur->next; }
    GenreLinkedlistNode* n = new GenreLinkedlistNode(g); n->next = genreHead; genreHead = n;
}

void MovieNode::populateGenres(const string& gstring) {
    if (gstring.empty() || gstring == "Unknown") return;
    int start = 0;
    for (int i = 0; i <= (int)gstring.size(); i++) {
        if (i == (int)gstring.size() || gstring[i] == '|') {
            string token = gstring.substr(start, i - start);
            int a = 0, b = (int)token.size() - 1;
            while (a <= b && token[a] == ' ') a++;
            while (b >= a && token[b] == ' ') b--;
            if (b - a + 1 > 0) addGenre(token.substr(a, b - a + 1));
            start = i + 1;
        }
    }
}

