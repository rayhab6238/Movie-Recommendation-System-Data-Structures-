#include "ActorHashTable.h"

unsigned long hashString(const string& s) {
    // Polynomial rolling hash with base 31 (prime), case-insensitive
    unsigned long hash = 0;
    unsigned long base = 31;
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        hash = (hash * base + (unsigned long)c) % 2147483647; // modulo large prime to prevent overflow
    }
    return hash;
}

ActorHashTable::ActorHashTable(int cap) {
    capacity = cap;
    count = 0;
    buckets = new BucketNode * [capacity];
    for (int i = 0; i < capacity; i++) buckets[i] = nullptr;
}

ActorHashTable::~ActorHashTable() {
    for (int i = 0; i < capacity; i++) {
        BucketNode* cur = buckets[i];
        while (cur) {
            BucketNode* nx = cur->next;
            delete cur->actor;
            delete cur;
            cur = nx;
        }
    }
    delete[] buckets;
}

ActorNode* ActorHashTable::find(const string& name) {
    unsigned long h = hashString(name) % capacity;
    BucketNode* cur = buckets[h];
    while (cur) {
        if (caseInsensitiveEqual(cur->actor->name, name)) return cur->actor;
        cur = cur->next;
    }
    return nullptr;
}

ActorNode* ActorHashTable::findPartial(const string& query) {
    if (query.size() == 0 || query.size() < 3) return nullptr;
    for (int i = 0; i < capacity; i++) {
        BucketNode* cur = buckets[i];
        while (cur) {
            if (caseInsensitiveContains(cur->actor->name, query)) return cur->actor;
            cur = cur->next;
        }
    }
    return nullptr;
}

ActorNode* ActorHashTable::getOrCreate(const string& name) {
    ActorNode* a = find(name);
    if (a) return a;
    ActorNode* newAct = new ActorNode(name);
    unsigned long h = hashString(name) % capacity;
    BucketNode* n = new BucketNode(newAct);
    n->next = buckets[h];
    buckets[h] = n;
    count++;
    return newAct;
}

