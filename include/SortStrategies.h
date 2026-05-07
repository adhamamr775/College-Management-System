#ifndef SORTSTRATEGIES_H
#define SORTSTRATEGIES_H

#include <vector>
#include "Department.h"
using namespace std;

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

#endif
