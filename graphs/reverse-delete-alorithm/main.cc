#include "reverse-delete.hpp"

int main ()
{
   std::list<edge<int>> input_list 
      {{5, 6, 40},
       {4, 5, 60},
       {5, 7, 70},
       {3, 4, 50},
       {1, 2, 30}, 
       {2, 3, 20}, 
       {3, 1, 10},
       {1, 7, 100},
       {1, 5, 5}};

   graph<int> my_graph {input_list};
   my_graph.graph_dump();
   my_graph.recursive_delete();
   my_graph.graph_dump();
}