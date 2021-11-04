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
    struct Stack
    {
        // Current size of stack.
        int size;

        // Stack bottom, also the start of the data sequence.
        int *bottom;

        // Maximum size of the stack.
        int MAX_SIZE = 8192;

        // Push an object into stack.
        void push(int x)
        {
            // If memory is not enough, than re-allocate them.
            if (size + 1 == MAX_SIZE - 1)
            {
                MAX_SIZE <<= 1;
                bottom = (int *)realloc(bottom, MAX_SIZE * sizeof(int));
            }

            bottom[size++] = x;
        }

        // Return if the stack is empty.
        bool empty()
        {
            return (size == 0);
        }

        // Return value of the top of the stack.
        int top()
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
                std::cerr << "Error stack size, stack cannot have negitive size!" << std::endl;
            }
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
            this->bottom = (int *)malloc(MAX_SIZE * sizeof(int));
            memcpy(this->bottom, target.bottom, size * sizeof(int));

            return *this;
        }

        // Constructor for the stack.
        Stack()
        {
            size = 0;
            bottom = (int *)malloc(MAX_SIZE * sizeof(int));
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
            // Check nullptr before memset.
            if (first_edge_id != nullptr)
            {
                memset(first_edge_id, 0, sizeof(*first_edge_id));
            }
        }
    };
}