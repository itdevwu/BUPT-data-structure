// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Standard header.
#include <fstream>

// Header for types.
#include "types.h"

namespace lab2
{
    // Set a graph by file input data.
    void set_graph_by_file(lab2::Graph &graph, char *file_name, std::fstream &fin)
    {
        int m;
        fin >> m;

        for (auto i = 1; i <= m; ++i)
        {
            int u, v, w;
            fin >> u >> v >> w;

            // Add edge.
            graph.add_edge(u, v, w);
            // Add reversed edge.
            graph.add_edge(v, u, w);
        }
    }

    // Set a graph by stdio input data.
    void set_graph_by_stdio(lab2::Graph &graph)
    {
        int m;
        std::cin >> m;

        for (auto i = 1; i <= m; ++i)
        {
            int u, v, w;
            std::cin >> u >> v >> w;

            // Add edge.
            graph.add_edge(u, v, w);
            // Add reversed edge.
            graph.add_edge(v, u, w);
        }
    }
}