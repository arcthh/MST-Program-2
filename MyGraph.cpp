#include <iostream>
#include "MyGraph.h"


ostream& operator<<(ostream& os, const Link& l)
{
   os << l.v1 << " " << l.v2 << " " << l.w;
   return os;
}

MyGraph::MyGraph(int n)
{
   this->nVert = n;
   this->adjacencyList.resize(n);
}

MyGraph::MyGraph(const MyGraph& g) : nVert(g.nVert), adjacencyList(g.adjacencyList) {}

bool MyGraph::addEdge(int a, int b, float w)
{
   if (a >= 1 && a <= nVert && b >= 1 && b <= nVert && a != b) {
         // Create a Link structure for the edge
         Link link;
         //ensure that v1 is smaller than v2 (personal preference for sorting)
         if (a < b) {
            link.v1 = a;
            link.v2 = b;
         } else {
            link.v1 = b;
            link.v2 = a;
         }
         link.w = w;

         // Check if the edge already exists in the adjacency list
         //this loop iterates through the adj list for v1, since it will point to v2.
         for (const Link& existingLink : adjacencyList[link.v1 - 1]) {
               if (existingLink.v2 == link.v2) {
                  return false; // Edge already exists
               }
         }
         // Add the edge to the adjacency list
         adjacencyList[link.v1 - 1].push_back(link);
         return true;
      }
      return false;
}

void MyGraph::output(std::ostream& os) {
    os << nVert << '\n';
   // Iterate through the adjacency list and print the edges
    for (int i = 0; i < nVert; i++) {
        for (const Link& link : adjacencyList[i]) {
            os << link.v1 << ' ' << link.v2 << ' ' << link.w << '\n';
        }
    }
}

pair<bool, float> MyGraph::weight(int a, int b)
{
   pair<bool, float> res;
   //default false until found
   res.first = false;
   res.second = (-1);

   int vertex1, vertex2;
   //ensure that v1 is smaller than v2 (personal preference for sorting)
   if (a < b) {
       vertex1 = a;
       vertex2 = b;
   } else {
       vertex1 = b;
       vertex2 = a;
   }

   //check link to see if it exists
   if (a>= 1 && a<= nVert && b>=1 && b<= nVert && a!=b) {
      for (const Link& link : adjacencyList[vertex1 - 1]) {
         if (link.v2 == vertex2) {
             return {true, link.w};
         }
      }
   return res;
   }
}

MyHelper::MyHelper()
{
}

vector<Link> Task1(int n, vector<Link>& pipes, MyHelper& helper)
{
   vector<Link> res;

   //BEGIN IMPLEMENTATION OF KRUSKALS ALGORITHM
   //sort in asc order
   sort(pipes.begin(), pipes.end());

   // Create a MyGraph object
   MyGraph graph = MyGraph(n);
    for (const Link& pipe : pipes) {
        if (graph.addEdge(pipe.v1, pipe.v2, pipe.w)) {
            res.push_back(pipe);
            // Check if we have added enough edges to form an MST (n-1 edges)
            if (res.size() == n - 1) {
                break;
            }
        }
    }
    helper.mst = res;
   return res;
}

pair<bool, Link> Task2(int n, vector<Link>& pipes, Link newPipe, MyHelper helper)
{
  Link l1;
  pair<bool, Link> sol;
  sol.first = true;
  sol.second = newPipe;
  return sol;
}