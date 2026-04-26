#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// ------------------- Utility Functions -------------------
//Searching should ignore case differences
//will return false in case of duplicates too
bool caseInsensitiveLess(const string& a, const string& b);
bool caseInsensitiveEqual(const string& a, const string& b);
bool caseInsensitiveContains(const string& a, const string& b);
string fixMissing(const char* s, const string& defaultValue);
void clean(char* s);
//stof() crashes on:
//Empty strings,Invalid numeric data
//Solution
//Safely converts string float ,Returns 0.0 if conversion fails
float safeStof(const string& str); //needed for IMDB rating

//A movie can have multiple genres
//Genres are dynamic(unknown count)
// ------------------- Genre Linked List -------------------
struct GenreLinkedlistNode {
    string genre;
    GenreLinkedlistNode* next;
    GenreLinkedlistNode(const string& g) : genre(g), next(nullptr) {}
};

// ------------------- Forward Declaration -------------------
class ActorNode;

// ------------------- Actor Linked List Node -------------------
struct ActorLinkedlistNode {
    ActorNode* actor;
    ActorLinkedlistNode* next;
    ActorLinkedlistNode(ActorNode* a) : actor(a), next(nullptr) {}
};

#endif

