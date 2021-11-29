#include "graph.h"

#define kDAMPENING 0.85

Graph::Graph()
{
    num_nodes_ = 0;
}

Graph::Graph(AdjList edges, vector<Node> nodes)
{
    edges_ = edges;
    nodes_ = nodes;
    num_nodes_ = nodes_.size();
}

Graph::Graph(string filename, size_t num_nodes)
{
    num_nodes_ = num_nodes;
    createNodeList();
    parseNodes(filename);
}

void Graph::DFS()
{
    vector<bool> visited;
    visited.resize(num_nodes_, false);

    connected_components_ = 0;
    for (size_t i = 0; i < num_nodes_; i++) {
        if(!visited[i]) {
            DFSHelper(i, visited);
            connected_components_++;
        }
    }
    cout << "Number of Components: " << connected_components_ << endl;
}

void Graph::DFSHelper(int id, vector<bool> &visited) {
    stack<int> futureVisits;
    futureVisits.push(id);

    while (!futureVisits.empty()) {
        int top = futureVisits.top();
        futureVisits.pop();    
        visited[top] = true;

        for (auto neighbor : edges_[top]) {
            int neighborId = neighbor->getId();
            if (!visited[neighborId]) {
                futureVisits.push(neighborId);
            }
        }
    }
}

void Graph::PageRank() const
{
    // Step 1 Create Matrix
    // Step 2 Choose Random Start Vector (Possibly just 1/N)
    // Step 3 Find Eigenvector
    // Step 4 Repeat Step 3 until at steady state vector
}

Matrix Graph::createGoogleMatrix() const {
    double positiveAdjustment = (1.0 - kDAMPENING) / num_nodes_;
    double noLinksInfluence = kDAMPENING / num_nodes_;

    vector<double> default_row;
    default_row.resize(num_nodes_, positiveAdjustment);
    vector<vector<double>> matrix;
    matrix.resize(num_nodes_, default_row);

    for (size_t c = 0; c < num_nodes_; c++) {
        if (edges_[c].size() == 0) {
            for (size_t r = 0; r < num_nodes_; r++) {
                matrix[r][c] += noLinksInfluence;
            }
        } else {
            double influence = kDAMPENING / edges_[c].size();
            for (auto node : edges_[c]) {
                matrix[node->getId()][c] += influence;
            }
        }
    }
    return matrix;
}

void Graph::BetweennessCentrality() const
{
}

void Graph::createNodeList() 
{
    nodes_.resize(num_nodes_, NULL);
    for (size_t i = 0; i < num_nodes_; i++) {
        nodes_[i] = Node(i);
    }
}

void Graph::parseNodes(string filename)
{
    edges_.resize(num_nodes_);

    ifstream data(filename);
    string edge;

    int from, to;
    if (data.is_open())
    {
        while (getline(data, edge))
        {
            if (edge[0] != '#') // Ignore Comments from input file
            {
                istringstream edgeStream;
                edgeStream.str(edge);
                edgeStream >> from;
                edgeStream >> to;

                edges_[from].push_back(&nodes_[to]);
            }
        }
    }
}

string Graph::outputEdges() const 
{
    string output;
    for (size_t i = 0; i < num_nodes_; i++) {
        output += '|' + to_string(i) + '|';
        for (auto node : edges_[i]) {
            output += " -> " + to_string(node->getId());
        }
        output += '\n';
    }
    return output;
}

unsigned Graph::getConnectedComponents() const {
    return connected_components_;
}