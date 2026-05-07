#include "../include/PrerequisiteGraph.h"
#include <queue>
#include <iostream>

void PrerequisiteGraph::addCourse(const string& courseCode) {
    // Insert the course if it doesn't exist yet
    if (adjList.find(courseCode) == adjList.end()) {
        adjList[courseCode] = vector<string>();
    }
}

void PrerequisiteGraph::addPrerequisite(const string& course, const string& prerequisite) {
    // Ensure both nodes exist
    addCourse(course);
    addCourse(prerequisite);

    // Add directed edge: course -> prerequisite
    adjList[course].push_back(prerequisite);
}

vector<string> PrerequisiteGraph::getPrerequisitesBFS(const string& courseCode) const {
    vector<string> result;

    // Check if the course exists in the graph
    if (adjList.find(courseCode) == adjList.end()) {
        return result;
    }

    queue<string> q;
    unordered_set<string> visited;

    // Start BFS from the given course
    q.push(courseCode);
    visited.insert(courseCode);

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        // Look up the adjacency list for the current course
        auto it = adjList.find(current);
        if (it != adjList.end()) {
            for (const string& prereq : it->second) {
                if (visited.find(prereq) == visited.end()) {
                    visited.insert(prereq);
                    result.push_back(prereq);
                    q.push(prereq);
                }
            }
        }
    }

    return result;
}

bool PrerequisiteGraph::dfsHelper(const string& node, unordered_set<string>& visited, unordered_set<string>& recStack) const {
    visited.insert(node);
    recStack.insert(node);

    auto it = adjList.find(node);
    if (it != adjList.end()) {
        for (const string& neighbor : it->second) {
            // If not visited, recurse
            if (visited.find(neighbor) == visited.end()) {
                if (dfsHelper(neighbor, visited, recStack)) {
                    return true;
                }
            }
            // If in recursion stack, we found a back-edge (cycle)
            else if (recStack.find(neighbor) != recStack.end()) {
                return true;
            }
        }
    }

    recStack.erase(node);
    return false;
}

bool PrerequisiteGraph::hasCycle() const {
    unordered_set<string> visited;
    unordered_set<string> recStack;

    // Run DFS from every unvisited node
    for (const auto& pair : adjList) {
        if (visited.find(pair.first) == visited.end()) {
            if (dfsHelper(pair.first, visited, recStack)) {
                return true;
            }
        }
    }

    return false;
}

void PrerequisiteGraph::displayGraph() const {
    cout << "Prerequisite Graph (Adjacency List):" << endl;
    for (const auto& pair : adjList) {
        cout << pair.first << " -> ";
        if (pair.second.empty()) {
            cout << "(no prerequisites)";
        }
        else {
            for (int i = 0; i < pair.second.size(); i++) {
                cout << pair.second[i];
                if (i < pair.second.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
}
