#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

class graph
{
    int n;
    bool** adjacency_map;

public:
    graph (const std::string&, const std::string&);
    ~graph();
    bool make_match (int, bool*, int*);
    int max_match ();
};

graph::graph (const std::string& enemy, const std::string& gregor) : n (enemy.size())
{   
    // Allocate space
    adjacency_map = new bool* [n];
    for (int i = 0; i < n; i++)
    {
        adjacency_map[i] = new bool[n];
        std::memset (adjacency_map[i], 0, sizeof(adjacency_map));    
    }

    // Fill the map
    for (int pos = 0; pos < n; pos++)
    {
        if (gregor[pos] == '0')
            continue;

        if (enemy[pos] == '0')
            adjacency_map[pos][pos] = true;
    
        if ((pos+1 != n) && (enemy[pos+1] == '1'))
            adjacency_map[pos][pos+1] = true;
        
        if ((pos-1 != -1) && enemy[pos-1] == '1')  
            adjacency_map[pos][pos-1] = true;  
    }
}

graph::~graph()
{
    // Free space
    for (int i = 0; i < n; i++)
        delete [] adjacency_map[i];    

    delete [] adjacency_map;
}

bool graph::make_match (int pawn, bool* seen, int* match)
{
    for (unsigned pos = 0; pos < n; pos++)
    {
        if (adjacency_map[pawn][pos] && !seen[pos])
        {
            seen[pos] = true; 

            // we take the pos if it's free or trying to find new pos for
            // gregor's pawn, which for now is determined for pos
            if (match[pos] == -1 || make_match (match[pos], seen, match))
            {
                match[pos] = pawn;
                return true;
            }
        }
    }
    return false;
}
 
int graph::max_match ()
{
    // i'ths enemy pos will be taken by match[i]'ths gregor's pawn
    // -1 means that no gregor's pawn was assined to i'ths enemy pos
    int match [n];
    std::memset (match, -1, sizeof (match));

    int result = 0; 

    // we go through all the gregor's pawns and call for each dfs-type 
    // algorithm that is looking for free enemy pos
    for (unsigned pawn = 0; pawn < n; pawn++)
    {
        // array to track the pos that we've already seen for fixed gregor pawn
        bool seen [n];
        std::memset (seen, 0, sizeof (seen));
 
        if (make_match (pawn, seen, match))
            result++;
    }
    return result;
}

int main ()
{
    int t;
    int n;
    std::string enemy;
    std::string gregor;
    std::vector<int> res_vec; 

    std::cin >> t;

    while (t--)
    {
        std::cin >> n;
        std::cin >> enemy;
        std::cin >> gregor;

        graph bipart_graph (enemy, gregor);
        res_vec.push_back (bipart_graph.max_match());
    }

    for (int res : res_vec)
        std::cout << res << std::endl;
}