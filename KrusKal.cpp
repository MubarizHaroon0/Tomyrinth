/// 22i-1222 shoaib irfan 22i-1104 Mubariz haroon  22i-0996
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <random>
#include <chrono>
#include <thread>

using namespace std;
namespace std
{
    template <>
    struct hash<pair<int, int>>
    {
        size_t operator()(const pair<int, int> &p) const
        {
            // Combine the hash values of the pair's components
            size_t hash1 = hash<int>{}(p.first);
            size_t hash2 = hash<int>{}(p.second);
            // Combine hash values using hash_combine function (boost's method)
            return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
        }
    };
}

// Define a structure to represent a cell in the maze
struct Cell
{
    int x, y; // Coordinates w.r.t to Maze
    int row, col;
    int set_id; // unique identifier for the set this cell belongs to
    bool top_wall, bottom_wall, left_wall, right_wall;
    Cell(int r = 0, int c = 0) : row(r), col(c), set_id(-1), top_wall(true), bottom_wall(true), left_wall(true), right_wall(true) {}
};

// Define a structure to represent a wall between two cells
struct Wall
{
    const Cell *cell1, *cell2;
    Wall(const Cell *c1, const Cell *c2) : cell1(c1), cell2(c2) {}
};

// Function to initialize the grid of cells
vector<vector<Cell>> initialize_grid(int rows, int cols)
{
    vector<vector<Cell>> grid(rows, vector<Cell>(cols, Cell(0, 0)));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j].row = i;
            grid[i][j].col = j;
        }
    }

    return grid;
}

// Function to initialize sets for each cell
void initialize_sets(vector<vector<Cell>> &grid)
{
    int set_id = 0;
    for (auto &row : grid)
    {
        for (auto &cell : row)
        {
            cell.set_id = set_id++;
        }
    }
}

// Function to find the set identifier of a cell
int find_set_id(const Cell *cell)
{
    return cell->set_id;
}

// Function to merge sets of two cells
// Function to merge sets of two cells
void merge_sets(Cell *cell1, Cell *cell2, vector<vector<Cell>> &grid)
{
    int set_id1 = find_set_id(cell1);
    int set_id2 = find_set_id(cell2);

    // Count the number of distinct sets before merging
    int num_sets_before = 0;
    for (auto &row : grid)
    {
        for (auto &cell : row)
        {
            if (cell.set_id == set_id1 || cell.set_id == set_id2)
            {
                num_sets_before++;
            }
        }
    }

    // Merge sets only if the merging decreases the number of sets
    if (num_sets_before > 1)
    {
        for (auto &row : grid)
        {
            for (auto &cell : row)
            {
                if (cell.set_id == set_id2)
                {
                    cell.set_id = set_id1;
                }
            }
        }
    }
}

// Function to generate all walls in the maze
vector<Wall> get_all_walls(const vector<vector<Cell>> &grid)
{
    vector<Wall> walls;
    int rows = grid.size();
    int cols = grid[0].size();

    // Add vertical walls
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols - 1; ++j)
        {
            walls.push_back(Wall(&grid[i][j], &grid[i][j + 1]));
        }
    }

    // Add horizontal walls
    for (int i = 0; i < rows - 1; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            walls.push_back(Wall(&grid[i][j], &grid[i + 1][j]));
        }
    }

    return walls;
}

// Function to remove wall between two cells
void remove_wall_between(Cell *&cell1, Cell *&cell2)
{
    // You can implement this function according to your data structure
    // For simplicity, let's just update the corresponding wall flags
    if (cell1->row == cell2->row)
    {
        if (cell1->col < cell2->col)
        {
            cell1->right_wall = false;
            cell2->left_wall = false;
        }
        else
        {
            cell1->left_wall = false;
            cell2->right_wall = false;
        }
    }
    else
    {
        if (cell1->row < cell2->row)
        {
            cell1->bottom_wall = false;
            cell2->top_wall = false;
        }
        else
        {
            cell1->top_wall = false;
            cell2->bottom_wall = false;
        }
    }
}

// Function to shuffle the walls randomly
void shuffle_walls(vector<Wall> &walls)
{
    // Use a random device to shuffle the walls
    random_device rd;
    mt19937 g(rd());
    shuffle(walls.begin(), walls.end(), g);
}

// Function to store maze data in a vector for further processing
vector<pair<pair<int, int>, vector<pair<int, int>>>> store_maze_data(const vector<vector<Cell>> &grid)
{
    vector<pair<pair<int, int>, vector<pair<int, int>>>> mazeData;

    for (const auto &row : grid)
    {
        for (const auto &cell : row)
        {
            pair<int, int> cellCoords = {cell.row, cell.col};
            vector<pair<int, int>> neighborsCoords;

            // Store coordinates of neighboring cells
            if (!cell.top_wall)
                neighborsCoords.emplace_back(cell.row - 1, cell.col);
            if (!cell.bottom_wall)
                neighborsCoords.emplace_back(cell.row + 1, cell.col);
            if (!cell.left_wall)
                neighborsCoords.emplace_back(cell.row, cell.col - 1);
            if (!cell.right_wall)
                neighborsCoords.emplace_back(cell.row, cell.col + 1);

            mazeData.emplace_back(make_pair(cellCoords, neighborsCoords));
        }
    }

    return mazeData;
}
// Function to find the shortest path using Breadth-First Search (BFS)
vector<pair<int, int>> find_shortest_path(const vector<pair<pair<int, int>, vector<pair<int, int>>>> &mazeData,
                                          pair<int, int> start, pair<int, int> end)
{
    queue<pair<int, int>> q;
    unordered_map<pair<int, int>, pair<int, int>> parent;

    q.push(start);
    parent[start] = {-1, -1};

    while (!q.empty())
    {
        pair<int, int> current = q.front();
        q.pop();

        if (current == end)
        {
            break;
        }

        for (const auto &neighbor : mazeData)
        {
            if (neighbor.first == current)
            {
                for (const auto &next : neighbor.second)
                {
                    if (!parent.count(next))
                    {
                        q.push(next);
                        parent[next] = current;
                    }
                }
                break;
            }
        }
    }

    vector<pair<int, int>> path;
    pair<int, int> current = end;
    while (current != make_pair(-1, -1))
    {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());

    return path;
}
