// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Please compile this program with G++ under C++ standard later than C++14.
// Example: " g++ lab2.cpp -std=c++14 -O2 -Wall -o lab2 ".

// Header for lab2.
#include "lab2.h"

// Main function.
int main(int argc, char *argv[])
{
    // Welcome.
    std::cout << "Welcome to the MAZE!\n\n";

    // Start and end vertices.
    int start_vertex, dest_vertex;

    // Graph to store the maze.
    lab2::Graph graph;

    // Stack to store the shortest route.
    lab2::Stack route_stack;
    // Stack to store temporary route.
    lab2::Stack tmp_stack;

    // Array to record if a node has been visited in a search.
    int *visited_node = (int *)malloc(1024 * sizeof(int));
    memset(visited_node, 0, sizeof(*visited_node));

    // Set graph by diverse input methods.
    if (lab2::handle_input(graph, start_vertex, dest_vertex, argc, argv))
    {
        tmp_stack.push(start_vertex);

        // Obtain result by dfs.
        int _result = lab2::dfs(start_vertex, dest_vertex, 0, INT32_MAX, visited_node, graph, tmp_stack, route_stack);

        // Judge if the destination is reached.
        if (_result == INT32_MAX)
        {
            // Print error message.
            std::cout << "Cannot find a way to destination." << std::endl;
        }
        else
        {
            // Print the shortest distance.
            std::cout << "Shortest distance of given problem is " << _result << " .\n";
            // Print the shortest route.
            lab2::print_route_stack(route_stack);
        }
    }

    // Return.
    return 0;
}
