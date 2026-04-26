#ifndef ACTORHASHTABLE_H
#define ACTORHASHTABLE_H

#include "utils.h"
#include "ActorNode.h"

unsigned long hashString(const string& s);

class ActorHashTable {
public:
    struct BucketNode {
        ActorNode* actor;
        BucketNode* next;
        BucketNode(ActorNode* a) : actor(a), next(nullptr) {}
    };

    BucketNode** buckets;
    int capacity;
    int count;

    ActorHashTable(int cap = 2048);
    ~ActorHashTable();

    ActorNode* find(const string& name);
    ActorNode* findPartial(const string& query);
    ActorNode* getOrCreate(const string& name);
};

#endif

