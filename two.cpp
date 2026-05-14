#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class ParallelSorting {
public:
    vector<int> arr;

    ParallelSorting(vector<int> v) {
        arr = v;
    }

    void printArray() {
        for (int x : arr) {
            cout << x << " ";
        }
        cout << endl;
    }

    // ---------------- SEQUENTIAL BUBBLE SORT ----------------
    void serialBubbleSort() {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    // ---------------- PARALLEL BUBBLE SORT ----------------
    // Odd-Even Transposition Sort
    void parallelBubbleSort() {
        int n = arr.size();

        for (int phase = 0; phase < n; phase++) {

            if (phase % 2 == 0) {
                // Even phase
#pragma omp parallel for
                for (int i = 0; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(arr[i], arr[i + 1]);
                    }
                }
            }
            else {
                // Odd phase
#pragma omp parallel for
                for (int i = 1; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(arr[i], arr[i + 1]);
                    }
                }
            }
        }
    }

    // ---------------- MERGE FUNCTION ----------------
    void merge(int left, int mid, int right) {

        vector<int> temp;

        int i = left;
        int j = mid + 1;

        while (i <= mid && j <= right) {
            if (arr[i] < arr[j]) {
                temp.push_back(arr[i++]);
            } else {
                temp.push_back(arr[j++]);
            }
        }

        while (i <= mid) {
            temp.push_back(arr[i++]);
        }

        while (j <= right) {
            temp.push_back(arr[j++]);
        }

        for (int k = 0; k < temp.size(); k++) {
            arr[left + k] = temp[k];
        }
    }

    // ---------------- SEQUENTIAL MERGE SORT ----------------
    void serialMergeSort(int left, int right) {

        if (left < right) {

            int mid = (left + right) / 2;

            serialMergeSort(left, mid);
            serialMergeSort(mid + 1, right);

            merge(left, mid, right);
        }
    }

    // ---------------- PARALLEL MERGE SORT ----------------
    void parallelMergeSort(int left, int right, int depth = 0) {

        if (left < right) {

            int mid = (left + right) / 2;

            if (depth <= 3) {

#pragma omp parallel sections
                {
#pragma omp section
                    parallelMergeSort(left, mid, depth + 1);

#pragma omp section
                    parallelMergeSort(mid + 1, right, depth + 1);
                }

            } else {

                parallelMergeSort(left, mid, depth + 1);
                parallelMergeSort(mid + 1, right, depth + 1);
            }

            merge(left, mid, right);
        }
    }
};

int main() {

    //vector<int> original = {5, 3, 10, 20, 0, -1};

    vector<int> arr(10000);

    for(int i = 0; i < 10000; i++) {
        arr[i] = rand() % 10000;
    }

    // Sequential Bubble Sort
    ParallelSorting s1(arr);

    double start = omp_get_wtime();
    s1.serialBubbleSort();
    double end = omp_get_wtime();

    cout << "Sequential Bubble Sort: ";
    //s1.printArray();
    cout << "Execution Time = " << (end - start) << " seconds\n\n";


    // Parallel Bubble Sort
    ParallelSorting p1(arr);

    start = omp_get_wtime();
    p1.parallelBubbleSort();
    end = omp_get_wtime();

    cout << "Parallel Bubble Sort: ";
    //p1.printArray();
    cout << "Execution Time = " << (end - start) << " seconds\n\n";


    // Sequential Merge Sort
    ParallelSorting s2(arr);

    start = omp_get_wtime();
    s2.serialMergeSort(0, arr.size() - 1);
    end = omp_get_wtime();

    cout << "Sequential Merge Sort: ";
    //s2.printArray();
    cout << "Execution Time = " << (end - start) << " seconds\n\n";


    // Parallel Merge Sort
    ParallelSorting p2(arr);

    start = omp_get_wtime();
    p2.parallelMergeSort(0, arr.size() - 1);
    end = omp_get_wtime();

    cout << "Parallel Merge Sort: ";
    //p2.printArray();
    cout << "Execution Time = " << (end - start) << " seconds\n";

    return 0;
}

