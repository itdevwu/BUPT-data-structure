// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Standard header.
#include <utility>

// Header for I/O.
#include "./lib/io.h"

namespace lab2
{
    int handle_input(lab2::Graph &graph,
                     int &start_vertex,
                     lab2::Stack<int> &dest_vertices,
                     int argc,
                     char *argv[])
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
            std::cin >> start_vertex;

            // Read all m destination vertices.
            int _m;
            std::cin >> _m;
            while (_m--)
            {
                int tmp;
                std::cin >> tmp;
                dest_vertices.push(tmp);
            }
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
            fin >> start_vertex;

            // Read all m destination vertices.
            int _m;
            fin >> _m;
            while (_m--)
            {
                int tmp;
                fin >> tmp;
                dest_vertices.push(tmp);
            }

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
    void sim_dfs_by_stack(lab2::Stack<int> final_dest,
                          lab2::Stack<std::pair<int, lab2::Route>> &dfs_sim_stack,
                          lab2::Graph &graph,
                          lab2::Route &best_route)
    {
        // If the stack is empty, there will be no reason to search.
        while (dfs_sim_stack.empty() != true)
        {
            // Pick the top vertex.
            auto cur_vertex = dfs_sim_stack.top();
            // Pop it out.
            dfs_sim_stack.pop();

            // Judge if it is our final destination.
            if (final_dest.exists(cur_vertex.first) == false)
            {
                // Enumerate each out-degree.
                for (int i = graph.first_edge_id[cur_vertex.first];
                     i != -1;
                     i = graph.data[i].next_edge_id)
                {
                    // Make sure destination of the edge would not have showed up in the route.
                    if (cur_vertex.second.route_stack.exists(graph.data[i].destination) == false)
                    {
                        // Extended route.
                        auto extend_route = cur_vertex.second;
                        // Add total distance.
                        extend_route.tot_distance += graph.data[i].distance;
                        // Add new vertex.
                        extend_route.route_stack.push(graph.data[i].destination);

                        // Push the new vertex into the stack along with its route.
                        dfs_sim_stack.push(
                            std::make_pair(
                                graph.data[i].destination,
                                extend_route));
                    }
                }
            }
            else
            {
                // Judge if this is the best route.
                if (cur_vertex.second < best_route)
                {
                    // Yes, then change the best one.
                    best_route = cur_vertex.second;
                }
            }
        }
    }
}