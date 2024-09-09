#include "splay_tree.hpp"

int main () 
{
   splay_tree<int> big_tree;
   big_tree.insert(3);
   big_tree.graph_dump();
   big_tree.insert(4);
   big_tree.graph_dump();
   big_tree.insert(5);
   big_tree.graph_dump();
   big_tree.insert(6);
   big_tree.graph_dump();
   big_tree.insert(3);
   big_tree.graph_dump();
   big_tree.insert(7);
   big_tree.graph_dump();

   splay_tree<int> small_tree;
   small_tree.insert(1);
   small_tree.graph_dump();
   small_tree.insert(0);
   small_tree.graph_dump();
   small_tree.insert(2);
   small_tree.graph_dump();
   small_tree.insert(0);
   small_tree.graph_dump();

   small_tree.merge(big_tree);
   small_tree.graph_dump();
   big_tree = small_tree.split(3);
   small_tree.graph_dump();
   big_tree.graph_dump();
}