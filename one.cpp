#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
public:
  int vertices;
  vector<vector<int>> adj_matrix;
  vector<bool> visited;

  Graph(int v) {
    this->vertices = v;
    this->adj_matrix.resize(v);
    this->visited.resize(v, false);
  }

  void add_edge(int u, int v) {
    adj_matrix[u].push_back(v);
    adj_matrix[v].push_back(u);
  }

  void reset_visited() { fill(visited.begin(), visited.end(), false); }

  void serial_bfs(int start_vertex) {
    queue<int> q;
    q.push(start_vertex);
    visited[start_vertex] = true;

    while (!q.empty()) {
      int node = q.front();
      q.pop();
      cout << node << " ";

      for (int neighbour : adj_matrix[node]) {
        if (!visited[neighbour]) {
          visited[neighbour] = true;
          q.push(neighbour);
        }
      }
    }
  }
  
  void serial_dfs(int start_vertex) {
    stack<int> s;
    s.push(start_vertex);
    visited[start_vertex] = true;

    while (!s.empty()) {
      int node = s.top();
      s.pop();
      cout << node << " ";

      for (auto it = adj_matrix[node].rbegin(); it != adj_matrix[node].rend();
           it++) {
        int neighbour = *it;
        if (!visited[neighbour]) {
          visited[neighbour] = true;
          s.push(neighbour);
        }
      }
    }
  }

  void parallel_bfs(int start_vertex) {
    queue<int> q;
    q.push(start_vertex);
    visited[start_vertex] = true;

    while (!q.empty()) {
      int level_size = q.size();
      vector<int> curr_level;

      for (int i = 0; i < level_size; i++) {
        int node = q.front();
        q.pop();
        cout << node << " ";
        curr_level.push_back(node);
      }

#pragma omp parallel for
      for (int i = 0; i < curr_level.size(); i++) {
        int node = curr_level[i];
        for (int neighbour : adj_matrix[node]) {
          if (!visited[neighbour]) {
            // ensure thread-safe access to visited[]
#pragma omp critical
            {
              visited[neighbour] = true;
              q.push(neighbour);
            }
          }
        }
      }
    }
  }

  void parallel_dfs_util(int vertex) {
    if (visited[vertex])
      return;

    visited[vertex] = true;
    cout << vertex << " ";

#pragma omp parallel for
    for (int i = 0; i < adj_matrix[vertex].size(); i++) {
      int u = adj_matrix[vertex][i];

#pragma omp critical
      {
        if (!visited[u]) {
#pragma omp task
          parallel_dfs_util(u);
        }
      }
    }
  }
 
  void parallel_dfs(int start_vertex) {
    // parallel section to process each neighbor of the start node
#pragma omp parallel
    {
#pragma omp single
      { 
        // ensures only one thread starts the root DFS
        parallel_dfs_util(start_vertex); }
      }
  }
};

int main() {
  Graph g = Graph(6);

  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 4);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  cout << "Serial BFS: ";
  double start = omp_get_wtime();
  g.serial_bfs(1);
  double end = omp_get_wtime();
  cout << endl << "Time: " << (end - start) * 1000 << "ms";
  g.reset_visited();

  cout << endl << "Serial DFS: ";
  start = omp_get_wtime();
  g.serial_dfs(1);
  end = omp_get_wtime();
  cout << endl << "Time: " << (end - start) * 1000 << "ms";
  g.reset_visited();

  cout << endl << "Parallel BFS: ";
  start = omp_get_wtime();
  g.parallel_bfs(1);
  end = omp_get_wtime();
  cout << endl << "Time: " << (end - start) * 1000 << "ms";
  g.reset_visited();

  cout << endl << "Parallel DFS: ";
  start = omp_get_wtime();
  g.parallel_dfs(1);
  end = omp_get_wtime();
  cout << endl << "Time: " << (end - start) * 1000 << "ms" << endl;

  return 0;
}
