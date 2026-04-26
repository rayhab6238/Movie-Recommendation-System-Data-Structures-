#include "utils.h"

//Sorting movie titles or actor names alphabetically, ignoring case.
//Convert each character to lowercase manually ('A' to 'a')
//Compare character by character
bool caseInsensitiveLess(const string& a, const string& b) {
    int n = min(a.size(), b.size());
    for (int i = 0; i < n; i++) {
        char ca = a[i]; if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        char cb = b[i]; if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
        if (ca < cb) return true;
        if (ca > cb) return false;
    }
    return a.size() < b.size();
}

bool caseInsensitiveEqual(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < (int)a.size(); i++) {
        char ca = a[i]; if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        char cb = b[i]; if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
        if (ca != cb) return false;
    }
    return true;
}
//Partial searches like search movies by part of the title
//Searching for genres or actor appearances
//Checks if string b is a substring of string a, ignoring case
bool caseInsensitiveContains(const string& a, const string& b) {
    if (b.size() == 0 || a.size() < b.size()) return false;
    for (size_t i = 0; i + b.size() <= a.size(); ++i) {
        bool ok = true;
        for (size_t j = 0; j < b.size(); ++j) {
            char ca = a[i + j]; char cb = b[j];
            if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
            if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
            if (ca != cb) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

string fixMissing(const char* s, const string& defaultValue) {
    if (!s || strlen(s) == 0) return defaultValue;
    return string(s);
}

void clean(char* s) {
    if (!s) return;
    int j = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c >= 32 && c <= 126) s[j++] = s[i];
    }
    s[j] = '\0';
}
//safely converts strings to float
float safeStof(const string& str) {
    if (str.empty() || str == "0") return 0.0f;
    try {
        return stof(str);
    }
    catch (...) {
        return 0.0f;
    }
}

