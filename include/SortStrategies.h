#ifndef SORTSTRATEGIES_H
#define SORTSTRATEGIES_H

#include <vector>
#include "Department.h"
#include "Course.h"
#include "Student.h"
using namespace std;

class BubbleSortStrategy {
public:

    void sortCourses(vector<Course>& courses) {

        int n = courses.size();

        for (int i = 0; i < n - 1; i++) {

            for (int j = 0; j < n - i - 1; j++) {

                if (courses[j].capacity > courses[j + 1].capacity) {

                    Course temp = courses[j];
                    courses[j] = courses[j + 1];
                    courses[j + 1] = temp;
                }
            }
        }
    }
};

class MergeSortStrategy {
public:

    void merge(vector<Department>& arr, int left, int mid, int right) {

        vector<Department> temp;

        int i = left;
        int j = mid + 1;

        while (i <= mid && j <= right) {

            if (arr[i].name <= arr[j].name) {
                temp.push_back(arr[i]);
                i++;
            }
            else {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid) {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= right) {
            temp.push_back(arr[j]);
            j++;
        }

        for (int k = left; k <= right; k++) {
            arr[k] = temp[k - left];
        }
    }

    void mergeSort(vector<Department>& arr, int left, int right) {

        if (left < right) {

            int mid = (left + right) / 2;

            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);

            merge(arr, left, mid, right);
        }
    }

    void sortDepartments(vector<Department>& departments) {

        mergeSort(departments, 0, departments.size() - 1);
    }
};

class SelectionSortStrategy {
public:

    void sortStudents(vector<Student>& students) {

        int n = students.size();

        for (int i = 0; i < n - 1; i++) {

            int minIndex = i;

            for (int j = i + 1; j < n; j++) {

                if (students[j].gpa < students[minIndex].gpa) {
                    minIndex = j;
                }
            }

            if (minIndex != i) {
                Student temp = students[i];
                students[i] = students[minIndex];
                students[minIndex] = temp;
            }
        }
    }
};

#endif
