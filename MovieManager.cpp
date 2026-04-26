#include "MovieManager.h"
#include <iostream>
using namespace std;

int MovieManager::height(MovieNode* n) {
    return n ? n->height : 0;
}

int MovieManager::getBalance(MovieNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

void MovieManager::addVerticesToGraph(MovieNode* node) {
    if (!node) return;
    addVerticesToGraph(node->left);
    graph->addVertex(node);
    addVerticesToGraph(node->right);
}

void MovieManager::buildEdgesUsingActorIndex() {
    int edgeCount = 0;

    for (int i = 0; i < actorTable.capacity; i++) {
        ActorHashTable::BucketNode* bucket = actorTable.buckets[i];

        while (bucket) {
            ActorNode* actor = bucket->actor;
            if (actor && actor->head) {
                ActorNode::MovieLinkedlistNode* movie1 = actor->head;

                while (movie1) {
                    ActorNode::MovieLinkedlistNode* movie2 = movie1->next;

                    while (movie2) {
                        graph->addEdge(movie1->movie, movie2->movie, "actor", actor->name);
                        edgeCount++;
                        movie2 = movie2->next;
                    }

                    movie1 = movie1->next;
                }
            }

            bucket = bucket->next;
        }

        if (i % 500 == 0 && i > 0) {
            cout << "Processed " << i << "/" << actorTable.capacity << " actor buckets...\n";
        }
    }

    cout << "Actor edges created: " << edgeCount << "\n";
}

void MovieManager::buildGenreEdges(MovieNode* node) {
    if (!node) return;

    buildGenreEdges(node->left);
    findMoviesWithSharedGenres(root, node);
    buildGenreEdges(node->right);
}

void MovieManager::findMoviesWithSharedGenres(MovieNode* compareNode, MovieNode* targetNode) {
    if (!compareNode || !targetNode || compareNode == targetNode) return;

    findMoviesWithSharedGenres(compareNode->left, targetNode);

    GenreLinkedlistNode* genre1 = targetNode->genreHead;
    while (genre1) {
        GenreLinkedlistNode* genre2 = compareNode->genreHead;
        while (genre2) {
            if (caseInsensitiveEqual(genre1->genre, genre2->genre)) {
                graph->addEdge(targetNode, compareNode, "genre", genre1->genre);
                return;
            }
            genre2 = genre2->next;
        }
        genre1 = genre1->next;
    }

    findMoviesWithSharedGenres(compareNode->right, targetNode);
}

MovieNode* MovieManager::rightRotate(MovieNode* y) {
    if (!y || !y->left) return y;
    MovieNode* x = y->left;
    MovieNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

MovieNode* MovieManager::leftRotate(MovieNode* x) {
    if (!x || !x->right) return x;
    MovieNode* y = x->right;
    MovieNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

MovieNode* MovieManager::insert(MovieNode* node, MovieNode* newNode) {

    if (!node)
        return newNode;

    if (caseInsensitiveLess(newNode->title, node->title)) {
        node->left = insert(node->left, newNode);
    }
    else if (caseInsensitiveLess(node->title, newNode->title)) {
        node->right = insert(node->right, newNode);
    }
    else {
        delete newNode;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && node->left && getBalance(node->left) >= 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && node->right && getBalance(node->right) <= 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && node->left && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && node->right && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void MovieManager::inOrder(MovieNode* node) {
    if (!node) return;
    inOrder(node->left);

    cout << "---------------------------------------\n";
    cout << "Title        : " << node->title << "\n";
    node->printActors();
    cout << "Year: " << node->year << " | Rating: " << node->score << "\n";

    inOrder(node->right);
}

void MovieManager::searchByTitle(MovieNode* node, string title) {
    if (!node) return;

    searchByTitle(node->left, title);

    if (caseInsensitiveEqual(node->title, title)) {
        cout << "---------------------------------------\n";
        cout << "Title        : " << node->title << "\n";
        cout << "Year         : " << node->year << "\n";
        cout << "Director     : " << node->director << "\n";
        cout << "Actor 1      : " << node->actor1 << "\n";
        cout << "Actor 2      : " << node->actor2 << "\n";
        cout << "Actor 3      : " << node->actor3 << "\n";
        cout << "Genres       : " << node->genres << "\n";
        cout << "Duration     : " << node->duration << " min\n";
        cout << "Budget       : " << node->budget << "\n";
        cout << "Rating       : " << node->score << "\n";
        cout << "---------------------------------------\n";
    }

    searchByTitle(node->right, title);
}

void MovieManager::searchByActorScan(MovieNode* node, const string& actor) {
    if (!node) return;
    searchByActorScan(node->left, actor);

    // check actor1, actor2, actor3 for exact or partial (case-insensitive)
    // For partial matching, require at least 2 characters to avoid false matches
    bool match1 = node->actor1.size() && (caseInsensitiveEqual(node->actor1, actor) || (actor.size() >= 2 && caseInsensitiveContains(node->actor1, actor)));
    bool match2 = node->actor2.size() && (caseInsensitiveEqual(node->actor2, actor) || (actor.size() >= 2 && caseInsensitiveContains(node->actor2, actor)));
    bool match3 = node->actor3.size() && (caseInsensitiveEqual(node->actor3, actor) || (actor.size() >= 2 && caseInsensitiveContains(node->actor3, actor)));
    
    if (match1 || match2 || match3) {
        cout << "Found (Actor - scan): " << node->title << "  [" << node->actor1 << ", " << node->actor2 << ", " << node->actor3 << "]\n";
    }

    searchByActorScan(node->right, actor);
}

void MovieManager::searchByGenre(MovieNode* node, string genre) {
    if (!node) return;
    searchByGenre(node->left, genre);

    string lowerGenres = node->genres;
    string lowerSearchGenre = genre;

    for (int i = 0; i < lowerGenres.size(); i++) {
        if (lowerGenres[i] >= 'A' && lowerGenres[i] <= 'Z')
            lowerGenres[i] = lowerGenres[i] - 'A' + 'a';
    }
    for (int i = 0; i < lowerSearchGenre.size(); i++) {
        if (lowerSearchGenre[i] >= 'A' && lowerSearchGenre[i] <= 'Z')
            lowerSearchGenre[i] = lowerSearchGenre[i] - 'A' + 'a';
    }

    if (lowerGenres.find(lowerSearchGenre) != string::npos)
        cout << "Found (Genre): " << node->title << " (" << node->year << ")\n";

    searchByGenre(node->right, genre);
}

void MovieManager::searchByYear(MovieNode* node, string year) {
    if (!node) return;
    searchByYear(node->left, year);

    if (node->year == year)
        cout << "Found (Year): " << node->title << "\n";

    searchByYear(node->right, year);
}

void MovieManager::searchByRating(MovieNode* node, float low, float high) {
    if (!node) return;

    searchByRating(node->left, low, high);

    // CRITICAL FIX: Use safeStof instead of stof
    float score = safeStof(node->score);
    if (score >= low && score <= high)
        cout << "Found (Rating): " << node->title << " (" << node->year << ") | " << score << "\n";

    searchByRating(node->right, low, high);
}

MovieNode* MovieManager::minValueNode(MovieNode* node) {
    MovieNode* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

MovieNode* MovieManager::findNodeByTitle(MovieNode* node, const string& title) {
    if (!node) return nullptr;
    if (caseInsensitiveEqual(node->title, title)) return node;
    if (caseInsensitiveLess(title, node->title))
        return findNodeByTitle(node->left, title);
    else
        return findNodeByTitle(node->right, title);
}

void MovieManager::removeMovieFromActors(MovieNode* m) {
    if (!m) return;
    ActorLinkedlistNode* ac = m->actorHead;
    while (ac) {
        if (ac->actor) ac->actor->removeMovie(m);
        ac = ac->next;
    }
}

MovieNode* MovieManager::deleteNode(MovieNode* root, string title) {
    if (!root) return root;

    if (caseInsensitiveLess(title, root->title))
        root->left = deleteNode(root->left, title);

    else if (caseInsensitiveLess(root->title, title))
        root->right = deleteNode(root->right, title);

    else {
        // found node to delete: 'root'
        if (!root->left || !root->right) {
            // 0 or 1 child
            MovieNode* temp = root->left ? root->left : root->right;

            // remove this movie's references from actor filmographies
            removeMovieFromActors(root);

            // Clear actorHead and genreHead before deletion to prevent destructor issues
            // The actors have already removed their references, so we just need to free the nodes
            if (root->actorHead) {
                ActorLinkedlistNode* ac = root->actorHead;
                while (ac) {
                    ActorLinkedlistNode* nx = ac->next;
                    delete ac;
                    ac = nx;
                }
                root->actorHead = nullptr;
            }
            if (root->genreHead) {
                GenreLinkedlistNode* gcur = root->genreHead;
                while (gcur) {
                    GenreLinkedlistNode* nx = gcur->next;
                    delete gcur;
                    gcur = nx;
                }
                root->genreHead = nullptr;
            }

            delete root;
            return temp;
        }
        else {
            // two children: use inorder successor
            MovieNode* succ = minValueNode(root->right);
            if (!succ) {
                // This shouldn't happen, but safety check
                delete root;
                return nullptr;
            }

            // Save successor's title BEFORE copying data (critical for deletion)
            string succTitle = succ->title;

            // --- Step 1: remove root's references from actors (they refer to root)
            removeMovieFromActors(root);

            // --- Step 2: free root's genre linked list (to avoid leak) and actorHead nodes
            // (we will replace them with successor's lists)
            if (root->genreHead) {
                GenreLinkedlistNode* gcur = root->genreHead;
                while (gcur) {
                    GenreLinkedlistNode* nx = gcur->next;
                    delete gcur;
                    gcur = nx;
                }
                root->genreHead = nullptr;
            }
            if (root->actorHead) {
                ActorLinkedlistNode* ac = root->actorHead;
                while (ac) {
                    ActorLinkedlistNode* nx = ac->next;
                    delete ac;
                    ac = nx;
                }
                root->actorHead = nullptr;
            }

            // --- Step 3: copy scalar fields from successor to root
            root->director = succ->director;
            root->duration = succ->duration;
            root->actor2 = succ->actor2;
            root->genres = succ->genres;
            root->actor1 = succ->actor1;
            root->title = succ->title;
            root->actor3 = succ->actor3;
            root->budget = succ->budget;
            root->year = succ->year;
            root->score = succ->score;

            // --- Step 4: move successor's lists to root
            root->genreHead = succ->genreHead; // transfer ownership
            root->actorHead = succ->actorHead; // transfer ownership

            // Clear successor's pointers so when we delete it later we don't double-free lists
            succ->genreHead = nullptr;
            succ->actorHead = nullptr;

            // --- Step 5: update actor filmographies to point from old successor pointer to root
            ActorLinkedlistNode* ac2 = root->actorHead;
            while (ac2) {
                ActorNode* an = ac2->actor;
                if (an) {
                    an->replaceMoviePointer(succ, root);
                }
                ac2 = ac2->next;
            }

            // --- Step 6: delete successor node from right subtree using saved title (it has at most one child)
            root->right = deleteNode(root->right, succTitle);
        }
    }

    // re-balance
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && root->left && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && root->left && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && root->right && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && root->right && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void MovieManager::destroyTree(MovieNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

MovieManager::MovieManager() : root(nullptr), actorTable(2048) {
    graph = new MovieGraph(4096);
}

void MovieManager::buildGraph() {
    cout << "\nBuilding movie graph...\n";
    addVerticesToGraph(root);
    cout << "Added all vertices. Building edges using actor index...\n";
    buildEdgesUsingActorIndex();
    cout << "Graph construction complete!\n";
    graph->displayStatistics();
}

void MovieManager::printMovieConnections(const string& title) {
    graph->printMovieConnections(title);
}

void MovieManager::displayGraphStats() {
    graph->displayStatistics();
}

void MovieManager::addMovie(MovieNode* m) {
    if (!m) return;
    
    // Save title and actor names before insertion (m may be deleted if duplicate)
    string movieTitle = m->title;
    string a1 = m->actor1;
    string a2 = m->actor2;
    string a3 = m->actor3;
    
    // Insert the movie (may delete m if duplicate title exists)
    root = insert(root, m);
    
    // Find the actual node in the tree (handles both new insert and duplicate cases)
    MovieNode* actualNode = findNodeByTitle(root, movieTitle);
    if (!actualNode) return; // Should not happen, but safety check
    
    // Update actor references via hash table and also add back-references into the movie
    if (a1.size() > 0 && a1 != "Unknown") {
        ActorNode* an1 = actorTable.getOrCreate(a1);
        an1->addMovie(actualNode);
        actualNode->addActorRef(an1);
    }
    if (a2.size() > 0 && a2 != "Unknown") {
        ActorNode* an2 = actorTable.getOrCreate(a2);
        an2->addMovie(actualNode);
        actualNode->addActorRef(an2);
    }
    if (a3.size() > 0 && a3 != "Unknown") {
        ActorNode* an3 = actorTable.getOrCreate(a3);
        an3->addMovie(actualNode);
        actualNode->addActorRef(an3);
    }
}

void MovieManager::display() {
    cout << "\n\n========== AVL TREE (In-Order) ==========\n";
    inOrder(root);
}

void MovieManager::searchByTitle(const string& title) {
    cout << "\nSearching for title: " << title << "\n";
    searchByTitle(root, title);
}

void MovieManager::searchByActor(const string& actor) {
    cout << "\nSearching for actor: " << actor << "\n";

    // 1) Try the hash table for an exact (case-insensitive) actor name
    ActorNode* an = actorTable.find(actor);
    if (an) {
        // exact match found in index ? print filmography (fast)
        an->printFilmography();
        return;
    }

    // 2) Try a partial match inside actor hash table (requires at least 2 characters)
    if (actor.size() < 2) {
        cout << "Query too short. Please enter at least 2 characters for partial search.\n";
        return;
    }
    
    ActorNode* ap = actorTable.findPartial(actor);
    if (ap) {
        cout << "Partial match found in index: " << ap->name << "\n";
        ap->printFilmography();
        return;
    }

    // 3) If not found exactly in index, fallback to a full scan (partial matches)
    cout << "Exact actor not found in index; doing full scan for partial matches...\n";
    searchByActorScan(root, actor);
}

void MovieManager::searchByGenre(const string& genre) {
    cout << "\nSearching for genre: " << genre << "\n";
    searchByGenre(root, genre);
}

void MovieManager::searchByYear(const string& year) {
    cout << "\nSearching for year: " << year << "\n";
    searchByYear(root, year);
}

void MovieManager::searchByRating(float low, float high) {
    cout << "\nMovies with rating between " << low << " and " << high << "\n";
    searchByRating(root, low, high);
}

void MovieManager::deleteMovie(const string& title) {
    root = deleteNode(root, title);
    cout << "\nMovie deleted (if existed).\n";
}

void MovieManager::printActorFilmography(const string& actor) {
    ActorNode* an = actorTable.find(actor);
    if (an) {
        an->printFilmography();
    }
    else {
        cout << "Actor not found: " << actor << "\n";
    }
}

void MovieManager::printActorCoActors(const string& actor) {
    ActorNode* an = actorTable.find(actor);
    if (an) {
        an->printCoActors();
    }
    else {
        cout << "Actor not found: " << actor << "\n";
    }
}

void MovieManager::recommendMovies(const string& title, int count) {
    graph->recommendMovies(title, count);
}

void MovieManager::findShortestPath(const string& movie1, const string& movie2) {
    graph->findShortestPath(movie1, movie2);
}

