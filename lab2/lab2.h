// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Standard header.

// Header for I/O.
#include "./lib/io.h"

namespace lab2
{
    // Print the route stack from bottom to top.
    void print_route_stack(lab2::Stack &route_stack)
    {
        // Note for users.
        std::cout << "*** PRINT ROUTE START ***\n";

        // Push the starting node into stack before dfs starts, thus its index is 0.
        std::cout << "Starting from vertex: " << route_stack.bottom[0] << std::endl;
        for (auto i = 1; i < route_stack.size; ++i)
        {
            printf("Step %d passed vertex: %d\n", i, route_stack.bottom[i]);
        }

        // Note for users.
        std::cout << "*** PRINT ROUTE END ***\n";
    }

    int handle_input(lab2::Graph &graph, int &start_vertex, int &dest_vertex, int argc, char *argv[])
    {
        // Status code, identifying what way user exactly chose to input.
        // 0 for error,
        // 1 for stdio,
        // 2 for file input.
        int _status = 0;

        // Receive input from stdio.
        if (argc == 1)
        {
            _status = 1;
#ifndef DISABLE_MESSAGE
            // Info for users.
            // The message can be hiden by adding " -DDISABLE_MESSAGE " to compile arguments.
            std::cout << "\nNote:\nYou are running this program without parameter,\n";
            std::cout << "    it will receive input from stdio.\n";
            std::cout << "\nIf you prefer using file as input source,\n";
            std::cout << "    you can add file name as parameter, such as \" ./lab2 testdata.txt \".\n";
            std::cout << "\nIf want to hide this message,\n";
            std::cout << "    you can add \" -DDISABLE_MESSAGE \" to compile arguments.\n\n";
#endif

            // Set the graph by input from stdio.
            set_graph_by_stdio(graph);

            // Set start and end nodes.
            std::cin >> start_vertex >> dest_vertex;
        }
        // Receive input from file.
        else if (argc == 2)
        {
            _status = 2;
#ifndef DISABLE_MESSAGE
            // Info for users.
            // The message can be hiden by adding " -DDISABLE_MESSAGE " to compile arguments.
            std::cout << "\nNote:\nYou are running this program with parameter,\n";
            std::cout << "    it will receive input from file.\n";
            std::cout << "\nIf you prefer using stdio as input source,\n";
            std::cout << "    you can run this program without parameter, such as \" ./lab2 \".\n";
            std::cout << "\nIf want to hide this message,\n";
            std::cout << "    you can add \" -DDISABLE_MESSAGE \" to compile arguments.\n\n";
#endif

            // Define a file stream for file I/O.
            std::fstream fin(argv[1]);

            // Set the graph by input from file.
            // Pass the file stream to the function by reference,
            // due to start/destination vertices info is at the end of the file.
            set_graph_by_file(graph, argv[1], fin);

            // Set start and end nodes.
            fin >> start_vertex >> dest_vertex;

            // Close file stream.
            fin.close();
        }
        // Invalid argument amount.
        else
        {
            _status = 0;
            // Error output.
            std::cerr << "Error: Too much or too few arguments: " << argc << " is not a valid amount.\n";
        }

        // Return status code.
        return _status;
    }

    // Use DFS to find the shortest route, Dijkstra or Bellman-Ford is better yet banned by tutors.
    // cur_node stands for current vertex's ID,
    // final_dest stands for final destination's ID,
    // tot_dist stands for current total distance of this specific DFS process,
    // ans stands for the best answer has been found at the moment,
    // visited_node stands for array recording if a node has been visited in this particular DFS,
    // graph stands for the graph,
    // route stands for route of current route,
    // best_route stands for route of the shortest route had been found at the moment,
    int dfs(
        int cur_node,
        int final_dest,
        int tot_dist,
        int ans,
        int *visited_node,
        lab2::Graph &graph,
        lab2::Stack &route,
        lab2::Stack &best_route)
    {
        // Judge if the current vertex is destination.
        if (cur_node == final_dest)
        {
            // If current result is better, than replace the existing one with it.
            if (tot_dist < ans)
            {
                ans = tot_dist;
                best_route = route;
            }
            // Return best answer.
            return ans;
        }

        // Mark this vertex is currently been occupied.
        visited_node[cur_node] = 1;
        // Flag to judge if this node is a dead end.
        bool is_end = true;

        // Enumerate each edge starting from current vertex.
        for (auto i = graph.first_edge_id[cur_node]; i != 0; i = graph.data[i].next_edge_id)
        {
            // Destination of this edge.
            int _dest = graph.data[i].destination;

            // Do the following if the destinations has not been occupied.
            if (!visited_node[_dest])
            {
                // That means this current node is not a dead end.
                is_end = false;

                // Add this new destination to route and DFS.
                route.push(_dest);
                int _result = dfs(_dest,
                                  final_dest,
                                  tot_dist + graph.data[i].distance,
                                  ans,
                                  visited_node,
                                  graph,
                                  route,
                                  best_route);

                // If this search reached the final destination.
                if (_result != -1)
                {
                    // Replace answer.
                    // No need to replace route since they are references.
                    ans = std::min(_result, ans);
                }
                // Pop this destination out of stack.
                route.pop();
            }
        }

        // Release this node.
        visited_node[cur_node] = 0;

        // Judge if this is an dead end.
        if (is_end)
        {
            // Return -1 since this search did not reach the final end.
            return -1;
        }
        else
        {
            // Return answer.
            return ans;
        }
    }
}