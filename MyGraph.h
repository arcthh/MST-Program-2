#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

struct Link {

public:

   int v1, v2;
   float w;

   //overloaded less than operator checks weights
   bool operator<(const Link &link2) const
   {
      return w < link2.w;
   }
   //checks for duplicity between links
   bool operator==(const Link &link2) const
   {
      return (v1 == link2.v1 && v2 == link2.v2 && w == link2.w) || ((v1 == link2.v2 && v2 == link2.v1 && w == link2.w));
   }

};

ostream& operator<<(ostream&, const Link&);

class MyGraph {

public:
   MyGraph(int n);
   MyGraph(const MyGraph&);

   bool addEdge(int a, int b, float w);
   void output(ostream& os);
   pair<bool, float> weight(int a, int b);

   //attributes
   int nVert;
   std::vector<std::list<Link>> adjacencyList;;

};

class MyHelper {

public:
    MyHelper();
    int x;
    vector<Link> mst;
};
int findParent(vector<int>& parent, int vertex);
vector<Link> Task1(int n, vector<Link>& pipes, MyHelper& helper);
pair<bool, Link> Task2(int n, vector<Link>& pipes, Link newPipe, MyHelper helper);