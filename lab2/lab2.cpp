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

    // Start vertex.
    int start_vertex;

    // Graph to store the maze.
    lab2::Graph graph;

    // Stack to store destinations.
    lab2::Stack<int> dest_vertices;

    // Stack to simulate DFS.
    lab2::Stack<std::pair<int, lab2::Route>> dfs_sim_stack;

    // Route to store the shortest route.
    lab2::Route best_route;
    // Init.
    best_route.tot_distance = INT32_MAX;

    // Set graph by diverse input methods.
    if (lab2::handle_input(graph, start_vertex, dest_vertices, argc, argv))
    {
        // Initiate a route.
        lab2::Route initial_route;
        // Push the start vertex into the route stack.
        initial_route.route_stack.push(start_vertex);

        // Start simulate DFS.
        dfs_sim_stack.push(std::make_pair(start_vertex, initial_route));

        // Obtain result by DFS simulated by a lab2::Stack.
        lab2::sim_dfs_by_stack(dest_vertices, dfs_sim_stack, graph, best_route);

        // Judge if the destination is reached.
        if (best_route.tot_distance == INT32_MAX)
        {
            // Print error message.
            std::cout << "Cannot find a way to destination." << std::endl;
        }
        else
        {
            // Print the shortest distance.
            std::cout << "The shortest distance is " << best_route.tot_distance << ".\n";
            // Print the shortest route.
            best_route.print_route_stack();
        }
    }

    // Return.
    return 0;
}