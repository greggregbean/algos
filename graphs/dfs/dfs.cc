#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <ctime>

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
bool vertex_in_list (const T& vertex, const std::list<T>& list) 
{
    for (auto i = list.begin(); i != list.end(); ++i) 
    {
        if (*i == vertex) 
            return true;
    }
    return false;
}

template <typename T>
void print_vector (const std::vector<T>& vector) 
{
    for (auto i = vector.begin(); i != vector.end(); ++i) 
    {
        std::cout << *i << " ";
    }

    std::cout << std::endl;
}

template <typename T>
class graph 
{
    // Adjacency map
    std::unordered_map <T, std::list<T>> map;

    void DFS (const T& vertex, std::vector<T>& visited) 
    {
        visited.push_back(vertex);
        for (auto i = map[vertex].begin(); i != map[vertex].end(); ++i) 
        {
            if (vertex_in_vector(*i, visited))
                continue;
            else
                DFS(*i, visited);
        }
    }

    public:
        graph () {}

        void insert (const T& vertex) 
        {
            map[vertex];
        }

        void insert (const T& vertex, const std::list<T>& preds, const std::list<T>& succs)
        {
            for (auto& pred : preds) 
            {
                map[pred].push_back(vertex);
            }
            auto succs_cpy = succs;
            map[vertex].merge(succs_cpy);
        }

        void insert (const T& vertex, const std::list<T>& friends) 
        {
            for (auto& fr : friends) 
            {
                map[fr].push_back(vertex);
            }
            auto friends_cpy = friends;
            map[vertex].merge(friends_cpy);
        }

        void make_directed_edge (const T& pred, const T& succ) 
        {
            map[pred].push_back(succ);
        }

        void make_edge (const T& vertex_1, const T& vertex_2) 
        {
            map[vertex_1].push_back(vertex_2);
            map[vertex_2].push_back(vertex_1);
        }

        std::vector<T> recursive_DFS (const T& root)
        {
            std::vector<T> visited;

            DFS (root, visited);

            return visited;
        }

        std::vector<T> non_recursive_DFS(const T& root) 
        {   
            T vertex;
            std::vector<T> visited;
            std::list<T> should_visit = {root};

            while (!should_visit.empty())
            {   
                vertex = *should_visit.begin();
                should_visit.pop_front();

                visited.push_back(vertex);

                for (auto i = map[vertex].begin(); i != map[vertex].end(); ++i)
                {    
                    if (!vertex_in_vector(*i, visited) && !vertex_in_list(*i, should_visit))
                        should_visit.push_back(*i);
                }
            }

            return visited;
        }
};



int main () 
{
    graph<int> full_mech_graph;
    std::list<int> vertex_list;

    for (int i = 0; i < 500; ++i) 
    {
        full_mech_graph.insert(i, vertex_list);
        vertex_list.push_back(i);
    }

    std::clock_t start_non_recursive = std::clock();
    std::vector<int> visited = full_mech_graph.non_recursive_DFS(0);
    std::clock_t end_non_recursive = std::clock();

    std::cout << "Non recursive DFS: ";
    print_vector(visited);
    std::cout << "Time elapsed: " << end_non_recursive - start_non_recursive << std::endl;

    visited = full_mech_graph.recursive_DFS(0);
    std::clock_t end_recursive = std::clock();

    std::cout << "Recursive DFS: ";
    print_vector(visited);
    std::cout << "Time elapsed: " << end_recursive - end_non_recursive << std::endl;
}

