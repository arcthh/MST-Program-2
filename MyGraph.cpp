#include <iostream>
#include "MyGraph.h"
//references
//https://www.geeksforgeeks.org/kruskals-algorithm-simple-implementation-for-adjacency-matrix/
//https://www.programiz.com/dsa/graph-adjacency-list

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

int findParent(vector<int>& parent, int vertex) {
    if (parent[vertex] == vertex) {
        return vertex;
    }
    return parent[vertex] = findParent(parent, parent[vertex]);
}


vector<Link> Task1(int n, vector<Link>& pipes, MyHelper& helper)
{

   // Define a parent array to track the parent of each vertex in the disjoint-set
   vector<int> parent(n);

   // Initialize the parent array such that each vertex is its own parent initially
   for (int i = 0; i < n; i++) {
      parent[i] = i;
   }

   vector<Link> res;

   //BEGIN IMPLEMENTATION OF KRUSKALS ALGORITHM
   //sort in asc order
   sort(pipes.begin(), pipes.end());

   // Create a MyGraph object
   MyGraph graph = MyGraph(n);
   //check for cycles
    for (const Link& pipe : pipes) {
         //checks to see if parents are the same or not
         int parent1 = findParent(parent, pipe.v1 - 1);
         int parent2 = findParent(parent, pipe.v2 - 1);

             if (parent1 != parent2 && graph.addEdge(pipe.v1, pipe.v2, pipe.w)) {
               // Adding this edge doesn't create a cycle
               // add to result and change parent
               res.push_back(pipe);
               parent[parent1] = parent2;
            }
            // Check if we have added enough edges to form an MST (n-1 edges)
            if (res.size() == n - 1) {
                break;
            }
    }
    helper.mst = res;
   return res;
}

/*
this function
takes in the number of cities (n), the original set of pipes, a newPipe, and the helper object that
is being passed, and return whether the new pipe will modify the original solution. If it does, the
pair of return will contains true, follow by the Link that is displaced. Otherwise it will return
false, and the Link can be anything (it will be ignored) by the main program.
Notice that in this case, the helper object is passed by value. So you cannot modify it inside the
function

The second task basically is asking if I add an edge to the graph, will it change the MST?
Obviously you can rerun the MST algorithm from scratch. However, if you may be able to speed
up the algorithm if you are willing to store some extra information when you create the MST, you
may be able to answer the question faster. Also remember the question I brought up in class:
“Given a tree, if you add one edge, how many cycle can it form?
*/

pair<bool, Link> Task2(int n, vector<Link>& pipes, Link newPipe, MyHelper helper)
{
   //load up the mst
   vector<Link> OGmst = helper.mst;
   // Define a parent array to track the parent of each vertex in the disjoint-set
   vector<int> parent(n);

   // Initialize the parent array such that each vertex is its own parent initially
   for (int i = 0; i < n; i++) {
      parent[i] = i;
   }


   //ensure  we don't modify
   vector<Link> pipesT2 = pipes;
   pipesT2.push_back(newPipe);

//checking
   // for (const Link &pipe : pipesT2) {
   //   cout << pipe.v1 << "->" << pipe.v2 << " " << pipe.w << "  ";
   // }


   //new solution
   vector<Link> sol2;

   //sort in asc order
   sort(pipesT2.begin(), pipesT2.end());

   // Create a MyGraph object
   MyGraph graph = MyGraph(n);
   //check for cycles
    for (const Link& pipe : pipes) {
         //checks to see if parents are the same or not
         int parent1 = findParent(parent, pipe.v1 - 1);
         int parent2 = findParent(parent, pipe.v2 - 1);

             if (parent1 != parent2 && graph.addEdge(pipe.v1, pipe.v2, pipe.w)) {
               // Adding this edge doesn't create a cycle
               // add to result and change parent
               sol2.push_back(pipe);
               parent[parent1] = parent2;
            }
            // Check if we have added enough edges to form an MST (n-1 edges)
            if (sol2.size() == n - 1) {
                break;
            }
    }

   //THIS PART IS NOT WORKING PROPERLY

    // Compare the two MSTs; If it is modified, they are not equal
    bool isModified = (OGmst == sol2);

    std::pair<bool, Link> sol;
    sol.first = isModified;

  return sol;
}