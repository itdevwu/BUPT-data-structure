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
                     int &dest_vertex,
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
    void sim_dfs_by_stack(int final_dest,
                          lab2::Stack<std::pair<int, lab2::Route>> &dfs_sim_stack,
                          lab2::Graph &graph,
                          lab2::Route &best_route)
    {
        while (dfs_sim_stack.empty() != true)
        {
            auto cur_vertex = dfs_sim_stack.top();
            printf("* %d\n", cur_vertex.first);
            dfs_sim_stack.pop();

            if (cur_vertex.first != final_dest)
            {
                for (int i = graph.first_edge_id[cur_vertex.first];
                     i != -1;
                     i = graph.data[i].next_edge_id)
                {
                    // printf("-- ^ %d\n", graph.data[i].destination);
                    // cur_vertex.second.print_route_stack();
                    if (cur_vertex.second.route_stack.exists(graph.data[i].destination) == false)
                    {
                        auto extend_route = cur_vertex.second;
                        extend_route.tot_distance += graph.data[i].distance;
                        extend_route.route_stack.push(graph.data[i].destination);

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
                    best_route = cur_vertex.second;
                }
            }
        }
    }
}