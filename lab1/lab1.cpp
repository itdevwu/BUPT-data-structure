// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Please compile this program with G++ under C++ standard later than C++14.
// Example: " g++ lab1.cpp -std=c++14 -O2 -Wall -o lab1 "

// Header for lab1.
#include "lab1.h"

// Main function.
int main(void)
{
    // Welcome.
    printf("*** Welcome to Josephus Ring ***\n\n");

    // Naming of N, S, M, X are following guidance of assignment's instruction.
    // N stands for total amount of people.
    // S stands for the person who is the start of the counting.
    // M stands for interval, meaning the M-th person to be called is supposed to die.
    // X stands for supposing amount of survivors.
    unsigned int N, S, M, X;

    // Define the Josephus Ring.
    lab1::JosephusRing ring;

    // Set value of N, S, M, X from stardard input data.
    lab1::set_NSMX(N, S, M, X);

    // Enumerate all people.
    // Set the Josephus Ring from people's data.
    lab1::set_ring(ring, N);

    // Attach head and tail of the Josephus Ring.
    ring.finalize();

    // Jump to the starting node.
    // See assignment's example: the M-th person should be killed. Thus "S - 1".
    ring.jump(S - 1);
    while ((N--) > X)
    {
        // Kill the M-th person counting from current node.
        // See assignment's example: the M-th person should be killed. Thus "M - 1".
        ring.jump(M - 1);
        // Remove the dead person of this round from the Josephus Ring.
        ring.remove_dead();
    }

    // Output survivors.
    ring.print();

    // Return.
    return 0;
}