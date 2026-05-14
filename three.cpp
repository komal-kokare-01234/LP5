#include<iostream>
#include<vector>
#include<omp.h>
using namespace std;
// take input 100000
// ================= SEQUENTIAL =================

int seq_min(vector<int>& arr)
{
    int min_val = arr[0];
    for(int i = 0; i < arr.size(); i++)
        if(arr[i] < min_val)
            min_val = arr[i];
    return min_val;
}

int seq_max(vector<int>& arr)
{
    int max_val = arr[0];
    for(int i = 0; i < arr.size(); i++)
        if(arr[i] > max_val)
            max_val = arr[i];
    return max_val;
}

long long seq_sum(vector<int>& arr)
{
    long long sum = 0;
    for(int i = 0; i < arr.size(); i++)
        sum += arr[i];
    return sum;
}

double seq_avg(vector<int>& arr)
{
    return (double)seq_sum(arr) / arr.size();
}

// ================= PARALLEL (OPTIMIZED) =================
/*
void parallel_all(vector<int>& arr, int &min_val, int &max_val, long long &sum)
{
    min_val = arr[0];
    max_val = arr[0];
    sum = 0;

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for(int i = 0; i < arr.size(); i++)
    {
        min_val = min(min_val, arr[i]);
        max_val = max(max_val, arr[i]);
        sum += arr[i];
    }
}
*/

int minPar(vector<int>& arr) {
    int min_val = arr[0];

    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] < min_val) min_val = arr[i];

    return min_val;
}

int maxPar(vector<int>& arr) {
    int max_val = arr[0];

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] > max_val) max_val = arr[i];

    return max_val;
}

long long sumPar(vector<int>& arr) {
    long long sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); i++)
        sum += arr[i];

    return sum;
}

double avgPar(vector<int>& arr) {
    return (double)sumPar(arr) / arr.size();
}

// ================= MAIN =================

int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    // Fill with random values
    for(int i = 0; i < n; i++)
        arr[i] = rand() % 1000;

    // Optional: control threads
    //omp_set_num_threads(4);

    // ----------- SEQUENTIAL -----------
    double start = omp_get_wtime();

    int min_s = seq_min(arr);
    int max_s = seq_max(arr);
    long long sum_s = seq_sum(arr);
    double avg_s = seq_avg(arr);

    double end = omp_get_wtime();

    cout << "\n--- Sequential Results ---\n";
    cout << "Min: " << min_s
         << "\nMax: " << max_s
         << "\nSum: " << sum_s
         << "\nAverage: " << avg_s << endl;
    cout << "Sequential Time: " << end - start << endl;

    // ----------- PARALLEL -----------
     start = omp_get_wtime();

    int min_p = minPar(arr);
    int max_p = maxPar(arr);
    long long sum_p = sumPar(arr);
    double avg_p = avgPar(arr);

     end = omp_get_wtime();

    cout << "\n--- Parallel Results ---\n";
    cout << "Min: " << min_p
         << "\nMax: " << max_p
         << "\nSum: " << sum_p
         << "\nAverage: " << avg_p << endl;
    cout << "Parallel Time: " << end - start << endl;

    return 0;
}