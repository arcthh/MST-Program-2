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
//CONSTRUCTOR to initialize a graph
MyGraph::MyGraph(int n)
{
   this->nVert = n;
   this->adjacencyList.resize(n);
}

//copy constructor
MyGraph::MyGraph(const MyGraph& g) : nVert(g.nVert), adjacencyList(g.adjacencyList) {}

//add edge
bool MyGraph::addEdge(int a, int b, float w)
{
   //checks to see that the vertices 'a' and 'b' are between the city limit and the number cities and are not equal
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
         //this loop iterates through the adj list for v1, since it will point to v2
         for (const Link& existingLink : adjacencyList[link.v1 - 1]) {
               if (existingLink.v2 == link.v2) {
                  return false; // Edge already exists
               }
         }
         // Add the edge to the adjacency list for its respective city
         adjacencyList[link.v1 - 1].push_back(link);
         return true;
      }
      return false;
}
//output function
void MyGraph::output(std::ostream& os) {
    os << nVert << '\n';
   // Iterate through the adjacency list and print the edges. Adjacency list is accessed through [][], so use range base for loop
    for (int i = 0; i < nVert; i++) {
        for (const Link& link : adjacencyList[i]) {
            os << link.v1 << ' ' << link.v2 << ' ' << link.w << '\n';
        }
    }
}
//weight function will return a boolean and the weight
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

   //check link to see if it exists, iterate through [][]
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
//function made to find a root of the vertex, uses recursion 
int findParent(vector<int>& parent, int vertex) {
    if (parent[vertex] == vertex) {
        return vertex; //if found return
    }
    return parent[vertex] = findParent(parent, parent[vertex]); //keeps searching
}

//task one is meant to be MST, i will be using Kruskals
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
         //checks to see if root node / parent are the same or not
         int parent1 = findParent(parent, pipe.v1 - 1);
         int parent2 = findParent(parent, pipe.v2 - 1);
            //if they do not share a root, they will not create a cycle
             if (parent1 != parent2 && graph.addEdge(pipe.v1, pipe.v2, pipe.w)) {
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

//task two works by checking if MST will be changed with addition of new pipe
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
   vector<Link> mstNew;

   //sort in asc order
   sort(pipesT2.begin(), pipesT2.end());

   // Create a MyGraph object
   MyGraph graph = MyGraph(n);
   //check for cycles
    for (const Link& pipe : pipesT2) {
         //checks to see if parents are the same or not
         int parent1 = findParent(parent, pipe.v1 - 1);
         int parent2 = findParent(parent, pipe.v2 - 1);

             if (parent1 != parent2 && graph.addEdge(pipe.v1, pipe.v2, pipe.w)) {
               // Adding this edge doesn't create a cycle
               // add to result and change parent
               mstNew.push_back(pipe);
               parent[parent1] = parent2;
            }
            // Check if we have added enough edges to form an MST (n-1 edges)
            if (mstNew.size() == n - 1) {
                break;
            }
    }
    // Compare the two MSTs; If it is modified, they are not equal
    //we have two msts, the original MST, and the mstNew
    //what i need to do is compare the two vectore, and determine which link is not in the new one, and if it is the new edge that did not get put in, return false with an empty ink
      std::pair<bool, Link> sol;

    sol.first = false;  // Assume the MSTs are initially equal

    // Compare the two MSTs element by element.
    for (size_t i = 0; i < mstNew.size(); i++) {
        const Link& l1 = mstNew[i];
        const Link& l2 = OGmst[i];

        // If any pair of links differs, the MSTs are not equal
        if (!(l1 == l2)) {
            sol.first = true;
            sol.second = l2;  // Return the  differing link
        }
    }
  return sol;
}