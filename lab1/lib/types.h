// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Header for auxiliary parts.
#include "etc.h"

// Namespace lab1.
namespace lab1
{
    // Struct in which a person's data is stored.
    struct Person
    {
        // ID of person.
        unsigned int id;
        // Age of person.
        unsigned int age;
        // Name of person.
        std::string name;
        // Gender of person (0 for MALE, 1 for female, 2 for LGBT).
        unsigned int gender;

        // Output data of this person to standard output.
        void print()
        {
            // String of noun of a specific gender.
            std::string _gender_str = "";
            switch (gender)
            {
                // Gender is male.
            case MALE:
                _gender_str = "MALE";
                break;
                // Gender is female.
            case FEMALE:
                _gender_str = "FEMALE";
                break;
                // Gender is transgender.
            case TRANSGENDER:
                _gender_str = "TRANSGENDER";
                break;
            }
            // Output a person's info
            printf("ID: %u, AGE: %u, NAME: %s, GENDER: %s\n", id, age, name.c_str(), _gender_str.c_str());
        }

        // Constructor of Person.
        Person(unsigned int set_id = 0, unsigned int set_age = 0, std::string set_name = "Unknown", unsigned int set_gender = MALE)
        {
            // Initalize data with given value.
            id = set_id;
            age = set_age;
            name = set_name;
            gender = set_gender;
        }
    };

    // Struct of nodes on the Josephus Ring.
    struct RingNode
    {
        // Data of person on this particular node.
        Person person;
        // Pointer to next node.
        RingNode *next_node;
        // Pointer to previous node.
        RingNode *previous_node;

        // Constructor for a node with empty information.
        RingNode()
        {
            // Initalize all poiters as nullptr.
            next_node = previous_node = nullptr;

            // Initalize all person as default.
            Person _tmp_person;
            person = _tmp_person;
        }

        // Construct a node with specific data of a person.
        RingNode(Person set_person, RingNode *set_next = nullptr, RingNode *set_previous = nullptr)
        {
            // Initalize data with given value.
            person = set_person;
            next_node = set_next;
            previous_node = set_previous;
        }
    };

    struct JosephusRing
    {
        // Starting node of the Josephus Ring.
        RingNode *start_node;
        // The node of the target person who is to die.
        RingNode *dead_node;

        // Remove the dead person from the Josephus Ring.
        void remove_dead()
        {
            // Output data of the person to die to standard output.
            printf("DEAD PERSON: ");
            dead_node->person.print();

            // If the starting person is to die, then change the starting node to his/her next node.
            if (dead_node == start_node)
            {
                start_node = start_node->next_node;
            }

            // Connect previous and next node of the dying person.
            dead_node->previous_node->next_node = dead_node->next_node;
            dead_node->next_node->previous_node = dead_node->previous_node;

            // Release memory of the node to die.
            RingNode *_tmp_node = dead_node;
            dead_node = dead_node->next_node;
            delete _tmp_node;
        }

        // Print data of the Josephus Ring at current status.
        void print()
        {
            // Alert users.
            printf("\n---PRINT SURVIVORS START---\n");

            // Move a temporary node to the final node.
            RingNode *_cur_node = start_node;
            _cur_node->person.print();
            // Judge if the current node is the last one.
            while (_cur_node->next_node != nullptr && _cur_node->next_node != start_node)
            {
                // Move node and output data.
                _cur_node = _cur_node->next_node;
                _cur_node->person.print();
            }

            // Alert users.
            printf("---PRINT SURVIVORS STOP---\n");
        }

        // Jump to the next node with a certain interval.
        void jump(unsigned int interval)
        {
            // Jump until the interval is zero.
            while (interval--)
            {
                // Jump to the next one.
                dead_node = dead_node->next_node;
            }
        }

        // Add a new person into the Josephus Ring.
        void add(Person target_person)
        {
            // Move a temporary node to the final node.
            RingNode *_cur_node = new RingNode;
            // Set a pointer to the temporary memory to be released when this function ends.
            RingNode *_delete_ptr = _cur_node;

            // Set the first node as the starting node of the ring.
            _cur_node->next_node = start_node;

            // Move a temporary node to the final node.
            // Judge if the current node is the last one.
            while (_cur_node->next_node != nullptr)
            {
                // Move to the next node.
                _cur_node = _cur_node->next_node;
            }

            // Add new person's node to tail of the sequence.
            _cur_node->next_node = new RingNode;
            // Attach new node's head with currently last node's tail.
            _cur_node->next_node->previous_node = _cur_node;
            _cur_node = _cur_node->next_node;
            // Set personal data for new node.
            _cur_node->person = target_person;

            // Release temporary memory.
            delete _delete_ptr;
        }

        // Attach head and tail of the ring.
        void finalize()
        {
            // Move a temporary node to the final node.
            RingNode *_cur_node = new RingNode;
            // Set a pointer to the temporary memory to be released when this function ends.
            RingNode *_delete_ptr = _cur_node;

            // The original starting person is the default result of constructor of struct "Person".
            // Remove the node created by struct JosephusRing's constructor by default.
            start_node = start_node->next_node;
            start_node->previous_node = nullptr;

            // Set the first node as the starting node of the ring.
            _cur_node->next_node = start_node;

            // Move a temporary node to the final node.
            // Judge if the current node is the last one.
            while (_cur_node->next_node != nullptr)
            {
                // Move to the next node.
                _cur_node = _cur_node->next_node;
            }

            // Connect the final node with the starting node.
            _cur_node->next_node = start_node;
            start_node->previous_node = _cur_node;

            // Initialize the dead node and make it equal to the starting node.
            dead_node = start_node;

            // Release temporary memory.
            delete _delete_ptr;
        }

        // Constructor of the Josephus Ring.
        JosephusRing()
        {
            // Initialize all pointers as null pointers.
            start_node = new RingNode;
            start_node->next_node = start_node->previous_node = nullptr;
            dead_node = start_node;
        }
    };
}