#include <fstream>
#include <iostream>

//====================
//= Splay-tree =======
//====================
   template <typename T>
   struct vertex 
   {
      T data;
      vertex<T>* left  = nullptr;
      vertex<T>* right = nullptr;
      vertex<T>* pred  = nullptr;

      vertex (T input_data) : data (input_data) {}
   };

   template <typename T>
   class splay_tree
   {
      vertex<T>* root = nullptr;

      vertex<T>* left_rotate  (vertex<T>* x);
      vertex<T>* right_rotate (vertex<T>* x);

      void splay (vertex<T>* x);
      vertex<T>* search (T data);

   public:
      splay_tree () {}
      splay_tree (vertex<T>* input_root) : root (input_root) {}
      splay_tree (T input_data) : root (new vertex<T> (input_data)) {}

      void insert (T data);
      bool remove (T data);
      void merge (splay_tree<T>& ntree);
      const vertex<T>* find (T data);
      splay_tree<T> split (T data);

      void graph_dump () const;
   };

//====================
//= Methods ==========
//====================
   template <typename T>
   void splay_tree<T>::insert (T data) 
   {
      vertex<T>* x = new vertex<T> (data);
      vertex<T>* pred = nullptr;
      vertex<T>* tmp_pred = root;

      // Finding pred for x
      while (tmp_pred != nullptr) 
      {
         pred = tmp_pred;

         if (x->data < tmp_pred->data)
            tmp_pred = tmp_pred->left;
         else 
            tmp_pred = tmp_pred->right;
      }

      x->pred = pred;

      // If tree is empty
      if (pred == nullptr) 
         root = x;

      // If tree is not empty
      else
      {
         if (x->data < pred->data)
            pred->left = x;
         else
            pred->right = x; 
         
         // Perform splaying to push x to root
         splay (x);
      }
   }

   template <typename T>
   vertex<T>* splay_tree<T>::left_rotate (vertex<T>* x) 
   {
      vertex<T>* y = x->right;
      if (y == nullptr) 
      {
         std::cout << "Left_rotate for " << x << "is impossible becasue it's right succ is NULL!" << std::endl;
         return nullptr;
      }
      x->right = y->left;

      if (y->left != nullptr)
         y->left->pred = x;

      y->pred = x->pred;
      
      if (x->pred == nullptr) 
         root = y;
      else if (x == x->pred->right)
         x->pred->right = y;
      else
         x->pred->left = y; 

      y->left = x;
      x->pred = y;

      return y;
   }

   template <typename T>
   vertex<T>* splay_tree<T>::right_rotate (vertex<T>* x) 
   {
      vertex<T>* y = x->left;
      if (y == nullptr)
      {
         std::cout << "Right_rotate for " << x << " is impossible becasue it's left succ is NULL!" << std::endl;
         return nullptr;
      }
      x->left = y->right;

      if (y->right != nullptr)
         y->right->pred = x;      

      y->pred = x->pred;

      if (x->pred == nullptr)
         root = y;
      else if (x == x->pred->right)
         x->pred->right = y;
      else
         x->pred->left = y; 

      y->right = x;
      x->pred = y;

      return y;
   }

   template <typename T>
   void splay_tree<T>::splay (vertex<T>* x) 
   {  
      vertex<T>* p; // pred
      vertex<T>* g; // grand-pred

      while (x != root)
      {
         p = x->pred;
         g = p->pred;

         // Zig case
         if (p == root)
         {
            if (x == p->left)
               right_rotate(x->pred);
            else // (x == p->right)
               left_rotate(x->pred);
         }
         
         else if (x == p->left)
         {  
            // Zig-zig case
            if (p == g->left)
               right_rotate (g);
            
            // Zig-zag case
            else // (p == g->right)
               right_rotate (p);
         }

         else // (x == p->right)
         {  
            // Zig-zig case
            if (p == g->right)
               left_rotate (g);
            
            // Zig-zag case
            else // (p == g->left)
               left_rotate (p);
         }
      }
   }

   template <typename T>
   vertex<T>* splay_tree<T>::search (T data)
   {
      vertex<T>* x = root;

      while ((x != nullptr) && (x->data != data))
      {
         if (data < x->data)
            x = x->left;
         else if (data > x->data)
            x = x->right;
      }

      if (x != nullptr)
         splay(x);

      return x;
   }

   template <typename T>
   const vertex<T>* splay_tree<T>::find (T data_req)
   {
      return search(data_req);
   }

   template <typename T>
   bool splay_tree<T>::remove (T data)
   {
      // FIND pushes vertex with DATA to the root, if it's in the tree
      vertex<T>* x = search(data);

      if (x != nullptr)
      {
         if (x->left != nullptr) 
         {
            x->left->pred = nullptr;
            root = x->left;
         }

         if (x->right != nullptr) {
            x->right->pred = nullptr; 
            splay_tree<T> ntree {x->right};
            merge (ntree);
         }

         delete x;
         return true;  
      }
 
      return false;
   }

   // In this function it's assumed that each element in NTREE > each element in THIS
   template <typename T>
   void splay_tree<T>::merge (splay_tree<T>& ntree)
   {  
      if (ntree.root != nullptr) 
      {
         vertex<T>* max = root;
         
         while (max->right != nullptr)
            max = max->right;

         // After pushing MAX to the root, it will have null right child
         // (since assuming that it won't, then it's not real max element in the tree)
         splay (max);

         root->right = ntree.root;
         ntree.root->pred = root;
         ntree.root  = nullptr;
      }
   }

   template <typename T>
   splay_tree<T> splay_tree<T>::split (T data)
   {
      vertex<T>* x = search (data);
      splay_tree<T> ntree {x->right};
      x->right->pred = nullptr;
      x->right = nullptr;

      return ntree;
   }


//====================
//= Graph dump =======
//====================
   template <typename T> 
   void dump_tree (std::ofstream& file, vertex<T>* x)
   {
      if (x == nullptr)
         return;
      
      file << std::dec  << long (x)        << "[shape=record, fillcolor = moccasin, style = filled, label = \" "
           << "{ Data:" << x->data  << " |"
           << "  Addr:" << x        << " |"
           << "  Pred:" << x->pred  << " |"
           << " {Left:" << x->left  << " |"
           << " Right:" << x->right << " }} \" ]" << std::endl;

      if (x->left != nullptr)
         file << long (x) << "->" << long (x->left)  << ";" << std::endl; 
      else
      {
         file << "null" << long (x) << "l [shape=point]" <<std::endl;
         file << long (x) << "-> null" << long (x) << "l;" << std::endl;
      }
      
      if (x->right != nullptr)
         file << long (x) << "->" << long (x->right) << ";" << std::endl;
      else
      {
         file << "null" << long (x) << "r [shape=point]" <<std::endl;
         file << long (x) << "-> null" << long (x) << "r;" << std::endl;
      }
         
      dump_tree (file, x->left);
      dump_tree (file, x->right);
   }

   template <typename T> 
   void splay_tree<T>::graph_dump () const 
   {
      std::ofstream file;
      file.open("./graph_pic/graph.dot", std::ofstream::out);
      file << "digraph { " << std::endl;
      dump_tree(file, root);
      file << "}" << std::endl;

      file.close();
   }

   
