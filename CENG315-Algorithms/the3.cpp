#include "the3.h"


// You  can define extra functions here

#define INF 999999999

using namespace std;

class The3 {
public:
  int n;
  int** edgeList;
  int** shortestPaths;
  double* scores;
  int numberOfComponents;

  The3(int n, int** edgeList, double* scores){
    this->n = n;
    this->edgeList = edgeList;
    this->scores = scores;
    this->numberOfComponents = 0;
  }

  void findComponents(){

    bool *visited = new bool[n];

    for(int v = 0; v < n; v++)
        visited[v] = false;

    for (int v=0; v<n; v++)
        if (visited[v] == false){
            helper(v, visited);
            numberOfComponents++;
        }
  }

  void helper(int v, bool* visited){
    visited[v] = true;
    for(int i=0; i<n; i++)
        if(edgeList[v][i] > 0 && !visited[i])
            helper(i, visited);
  }

  void convertEdgeList(){
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i!=j && edgeList[i][j] == 0)
              edgeList[i][j] = INF;
  }

  void floydWarshall() {

    shortestPaths = new int*[n];

    for (int l = 0; l < n; ++l)
      shortestPaths[l] = new int[n];

    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        shortestPaths[i][j] = edgeList[i][j];

    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          if (shortestPaths[i][k] + shortestPaths[k][j] < shortestPaths[i][j])
              shortestPaths[i][j] = shortestPaths[i][k] + shortestPaths[k][j];
  }

  void calculateScores(){

    for (int i = 0; i < n; i++)
      scores[i] = 0;

    for (int v = 0; v < n; v++)
      for (int s = 0; s < n; s++)
        for (int t = 0; t < n; t++) {
          int st, sv, vt;
          st = shortestPaths[s][t];
          sv = shortestPaths[s][v];
          vt = shortestPaths[v][t];
          if (s == t)
            st=1;
          if (v == s || v == t)
            sv = vt = 0;
          if (st != INF && sv != INF && vt != INF)
            scores[v] += (double)(sv+vt)/st;
        }
  }

};

// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value :
//                 number of disconnected components
int Important (int n, int**& edgeList, double*& scores)
{
  The3 the3(n, edgeList, scores);
  the3.findComponents();
  the3.convertEdgeList();
  the3.floydWarshall();
  the3.calculateScores();
  return the3.numberOfComponents;
}
