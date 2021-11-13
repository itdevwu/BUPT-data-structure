// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Standard headers.
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>

namespace lab2
{
    // Stack of integer.
    template <class _T>
    struct Stack
    {
        // Current size of stack.
        int size;

        // Stack bottom, also the start of the data sequence.
        _T *bottom;

        // Maximum size of the stack.
        int MAX_SIZE = 8192;

        // Push an object into stack.
        void push(_T x)
        {
            // If memory is not enough, than re-allocate them.
            if (size + 1 == MAX_SIZE - 1)
            {
                MAX_SIZE <<= 1;
                bottom = (_T *)realloc(bottom, MAX_SIZE * sizeof(_T));
            }

            bottom[size++] = x;
        }

        // Return if the stack is empty.
        bool empty()
        {
            return (size == 0);
        }

        // Return value of the top of the stack.
        _T top()
        {
            return bottom[size - 1];
        }

        // Pop the top object from the stack.
        void pop()
        {
            // Check is size is a negitive number.
            if (size > 0)
            {
                --size;
            }
            else
            {
                // Error message.
                std::cerr << "Error stack size, stack cannot have negitive size!" << std::endl;
            }
        }

        // Judge if an object exists in the stack.
        bool exists(_T target)
        {
            // Default result = false.
            bool _result = false;
            for (auto i = 0; i < size; ++i)
            {
                // If found one.
                if (bottom[i] == target)
                {
                    // Make result true and break.
                    _result = true;
                    break;
                }
            }
            return _result;
        }

        // Operator = overloading for correct "=" behavior.
        Stack operator=(const Stack &target)
        {
            // Judge if the target is itself.
            if (this == &target)
            {
                return *this;
            }

            this->size = target.size;
            this->MAX_SIZE = target.MAX_SIZE;
            this->bottom = (_T *)malloc(MAX_SIZE * sizeof(_T));
            memcpy(this->bottom, target.bottom, size * sizeof(_T));

            return *this;
        }

        // Constructor for the stack.
        Stack()
        {
            size = 0;
            bottom = (_T *)malloc(MAX_SIZE * sizeof(_T));
        }
    };

    // Struct for edge.
    struct edge
    {
        // ID of the next edge that has same origin of this edge.
        int next_edge_id;
        // Value of this edge.
        int distance;
        // Destination of this edge.
        int destination;
    };

    struct Graph
    {
        // Edges in this graph.
        edge *data;

        // ID of the first edge of each vertex.
        int *first_edge_id;

        // Amount of edges.
        int edge_count;

        // Maximum size of the graph.
        int MAX_SIZE;
        // Maximum ID of vertex.
        int MAX_ORIGIN_ID;

        // Add an edge to the graph.
        // Origin stands for start of an edge,
        // destination stands for end of an edge,
        // distance stands for value of an edge.
        void add_edge(int origin, int desination, int distance)
        {
            // If memory is not enough, than re-allocate them.
            if (origin + 1 == MAX_ORIGIN_ID - 1)
            {
                // Expand by 200%.
                MAX_ORIGIN_ID <<= 1;
                first_edge_id = (int *)realloc(first_edge_id, MAX_ORIGIN_ID * sizeof(int));
            }

            // If memory is not enough, than re-allocate them.
            if (edge_count + 1 == MAX_SIZE - 1)
            {
                // Expand by 200%.
                MAX_SIZE <<= 1;
                data = (edge *)realloc(data, MAX_SIZE * sizeof(edge));
            }

            // Add this edge's data into the data array.
            data[++edge_count].destination = desination;
            data[edge_count].distance = distance;
            data[edge_count].next_edge_id = first_edge_id[origin];
            // Create new "first edge" for the origin vertex.
            first_edge_id[origin] = edge_count;
        }

        // Constructor for graph.
        Graph()
        {
            edge_count = 0;
            MAX_SIZE = 8192;
            MAX_ORIGIN_ID = 8192;
            data = (edge *)malloc(MAX_SIZE * sizeof(edge));
            first_edge_id = (int *)malloc((MAX_ORIGIN_ID + 1) * sizeof(int));

            // Initalize first edge array.
            std::fill(first_edge_id, first_edge_id + MAX_SIZE, -1);
        }
    };

    struct Route
    {
        int tot_distance;
        lab2::Stack<int> route_stack;

        // Print the route stack from bottom to top.
        void print_route_stack()
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

        Route operator=(const Route &target)
        {
            // Judge if the target is itself.
            if (this == &target)
            {
                return *this;
            }

            this->tot_distance = target.tot_distance;
            this->route_stack = target.route_stack;

            return *this;
        }

        bool operator<(const Route &target)
        {
            return (this->tot_distance < target.tot_distance);
        }

        bool operator==(const Route &target)
        {
            return (this->tot_distance == target.tot_distance);
        }

        bool operator>(const Route &target)
        {
            return (this->tot_distance > target.tot_distance);
        }

        Route(int dist, lab2::Stack<int> sta)
        {
            tot_distance = dist;
            route_stack = sta;
        }

        Route()
        {
            tot_distance = 0;
        }
    };
}