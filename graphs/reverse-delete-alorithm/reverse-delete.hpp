#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <unordered_map>
#include <climits>

//-------------------------------------------------
// Declaration
//-------------------------------------------------
template <typename T>
struct edge
{
   T first;
   T second;
   unsigned weight;

   edge () {}
   edge (T&& v1, T&& v2, unsigned w) : 
      first  (std::move (v1)), 
      second (std::move (v2)),
      weight (w) {}
   edge (const T& v1, const T& v2, unsigned w) : 
      first  (v1), 
      second (v2),
      weight (w) {}
};


template <typename T>
class graph
{
   std::list<edge<T>> edge_list;
   std::unordered_map<T,std::list<T>> map;

public:
   graph () {};
   graph (const std::list<edge<T>>& list) 
      {push_back_list(list);}
   graph (std::list<edge<T>>&& list) 
      {push_back_list(list);}

   void push_back_edge (const edge<T>& e);
   void push_back_edge (edge<T>&& e);
   void push_back_list (const std::list<edge<T>>& list) 
      {for (auto& e : list){push_back_edge (e);}}
   void push_back_list (std::list<edge<T>>&& list)      
      {for (auto& e : list){push_back_edge (e);}}

   void dump ();
   void graph_dump (); 
   unsigned find_weight (const T& first, const T& second);
   void DFS_dump_graph (std::ofstream& file, const T& vertex, std::vector<T>& visited);
   bool check_connectivity (const T& root_DFS);
   void recursive_delete ();
   void DFS (const T& vertex, std::vector<T>& visited);
};


//-------------------------------------------------
// Definition
//-------------------------------------------------
template <typename T>
void graph<T>::push_back_edge (const edge<T>& e) 
{
   edge_list.push_back (e);
   map[e.first].push_back (e.second);
   map[e.second].push_back (e.first);
}

template <typename T>
void graph<T>::push_back_edge (edge<T>&& e) 
{
   edge_list.push_back (e);
   map[e.first].push_back (e.second);
   map[e.second].push_back (e.first);
}

template <typename T>
bool vertex_in_vector (const T& vertex, const std::vector<T>& vector) 
{
    for (auto i = vector.begin(); i != vector.end(); ++i) 
    {
        if (*i == vertex) 
            return true;
    }
    return false;
}

template <typename T>
void graph<T>::DFS (const T& vertex, std::vector<T>& visited) 
{
   visited.push_back(vertex);
   for (auto i = map[vertex].begin (); i != map[vertex].end (); ++i) 
   {
      if (vertex_in_vector (*i, visited))
            continue;
      else
            DFS (*i, visited);
   }
}

template <typename T>
bool graph<T>::check_connectivity (const T& root_DFS)
{
   std::vector<T> visited;
   unsigned num_of_v = map.size();

   DFS (root_DFS, visited);

   return (num_of_v == visited.size ());
}


template <typename T>
void graph<T>::recursive_delete ()
{
   edge<T> tmp_e;
   T root_DFS;
   
   edge_list.sort([](const edge<T>& a, const edge<T>& b) -> bool
      {return a.weight > b.weight;});

   auto it = edge_list.begin ();
   
   while (it != edge_list.end ())
   {
      tmp_e = *it;

      map[tmp_e.first].remove (tmp_e.second);
      map[tmp_e.second].remove (tmp_e.first);

      root_DFS = tmp_e.first;
      if (check_connectivity (root_DFS)) {
         it = edge_list.erase (it);
         graph_dump();
      }
         
      else 
      {
         map[tmp_e.first].push_back (tmp_e.second);
         map[tmp_e.second].push_back (tmp_e.first);
         it++;
      }    
   }
   
}

template <typename T>
void graph<T>::dump ()
{
      std::cout << "{" << std::endl;
      for (auto& e : edge_list)
      {
         std::cout << "   [ "   << e.first  << " <--|" << e.weight  
                   << "|--> " << e.second << " ]"    << std::endl;
      }
      std::cout << "}" << std::endl;
}

template <typename T>
unsigned graph<T>::find_weight (const T& first, const T& second)
{
   for (auto& e : edge_list)
   {
      if ( (e.first == first  && e.second == second) ||
           (e.first == second && e.second == first)  )
         return e.weight;
   }

   return UINT_MAX;
}


template <typename T>
void graph<T>::DFS_dump_graph (std::ofstream& file, const T& vertex, std::vector<T>& visited)
{
   file << "   " << vertex << " [shape=circle, fillcolor = moccasin, style = filled]" << std::endl;
   visited.push_back(vertex);
   for (auto i = map[vertex].begin (); i != map[vertex].end (); ++i) 
   {
      if (vertex_in_vector (*i, visited)) 
      {
         file << "   " << vertex << " -- " << *i << "[ label= \"" 
              << find_weight(vertex, *i) << "\" ];" << std::endl;
         continue;
      }
      else
      {
         DFS_dump_graph(file, *i, visited);
      }
   }
}

template <typename T> 
void graph<T>::graph_dump () 
{
   std::ofstream file;
   file.open("./graph_pic/graph.dot", std::ofstream::out);
   file << "strict graph { " << std::endl
        << "rankdir=LR;"     << std::endl;
   T root = edge_list.front().first;
   std::vector<T> visited;
   DFS_dump_graph(file, root, visited);
   file << "}" << std::endl;

   file.close();
}

