#ifndef PREREQUISITEGRAPH_H
#define PREREQUISITEGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class PrerequisiteGraph {
private:
    // Adjacency list: course code -> list of prerequisite course codes
    unordered_map<string, vector<string>> adjList;

    // Helper for DFS cycle detection
    bool dfsHelper(const string& node, unordered_set<string>& visited, unordered_set<string>& recStack) const;

public:
    // Add a course node to the graph
    void addCourse(const string& courseCode);

    // Add a prerequisite edge: course requires prerequisite
    void addPrerequisite(const string& course, const string& prerequisite);

    // BFS: Get all prerequisites (direct and indirect) for a course
    vector<string> getPrerequisitesBFS(const string& courseCode) const;

    // DFS: Detect if adding a prerequisite would create a cycle
    bool hasCycle() const;

    // Display the adjacency list
    void displayGraph() const;
};

#endif
