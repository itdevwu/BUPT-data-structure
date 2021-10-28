// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Header for types.
#include "./lib/types.h"

// Namespace lab1.
namespace lab1
{
    // Set value for N, S, M, X, after which are named due to reasons listed in comment of main function.
    void set_NSMX(unsigned int &N, unsigned int &S, unsigned int &M, unsigned int &X)
    {
        // Buffer for input, used to handling error input.
        std::string _buffer_str;

        // Request iuput for N.
        while (1)
        {
            // Request iuput for N.
            printf("Input amount of people who consist the Josephus Ring:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Convert buffer data into rquired data.
            auto _tmp_uint = lab1::uint_parser(_buffer_str);
            // Judge if buffer string is a positive integer.
            if (lab1::validate_uint(_buffer_str) && _tmp_uint > 0)
            {
                N = _tmp_uint;
                break;
            }
            else
            {
                printf("Error: Input amount of people is invalid, make sure it is a positive integer and retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
            }
        }

        // Request iuput for S.
        while (1)
        {
            printf("Input the person who is the start of the counting:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Convert buffer data into rquired data.
            auto _tmp_uint = lab1::uint_parser(_buffer_str);
            // Judge if buffer string is a positive integer less than or equal to total amount.
            if (lab1::validate_uint(_buffer_str) && _tmp_uint > 0 && _tmp_uint <= N)
            {
                S = _tmp_uint;
                break;
            }
            else
            {
                printf("Error: Input starting person is invalid, make sure it is a positive integer less than or equal to total amount of people and retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
            }
        }

        // Request iuput for M.
        while (1)
        {
            printf("Input interval of the counting:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Convert buffer data into rquired data.
            auto _tmp_uint = lab1::uint_parser(_buffer_str);
            // Judge if buffer string is a positive integer.
            if (lab1::validate_uint(_buffer_str) && _tmp_uint > 0)
            {
                M = _tmp_uint;
                break;
            }
            else
            {
                printf("Error: Input starting person is invalid, make sure it is a positive integer and retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
            }
        }

        // Request iuput for X.
        while (1)
        {
            printf("Input supposing amount of survivors:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Convert buffer data into rquired data.
            auto _tmp_uint = lab1::uint_parser(_buffer_str);
            // Judge if buffer string is an unsigned integer less than or equal to total amount.
            if (lab1::validate_uint(_buffer_str) && _tmp_uint >= 0 && _tmp_uint <= N)
            {
                X = _tmp_uint;
                break;
            }
            else
            {
                printf("Error: Input starting person is invalid, make sure it is an unsigned integer less than or equal to amount of people and retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
            }
        }
    }

    // Set value for a struct Person from standard input.
    void set_ring(lab1::JosephusRing &ring, unsigned int person_cnt)
    {
        printf("Personal data input:\n");

        // Buffer for input, used to handling error input.
        std::string _buffer_str;

        // Request input for each person's data.
        for (unsigned int i = 1; i <= person_cnt; ++i)
        {
            printf("\nInput %s person's data:\n", lab1::cardinal_to_ordinal(i).c_str());

            // Temporary Person struct to handle input data.
            lab1::Person tmp_person;

            // Id of this person.
            printf("ID:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Judge if buffer string is a valid ID.
            if (!lab1::validate_uint(_buffer_str))
            {
                // Error message.
                printf("Error: Your input of ID is invalid (must be an unsigned integer), please retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
                // Minus 1 for retry.
                --i;
                continue;
            }
            else
            {
                // Convert buffer string to required value and stored into the struct Person.
                tmp_person.id = lab1::uint_parser(_buffer_str);
            }

            // Name of this person.
            printf("Name:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Remove leading and trailing spaces.
            lab1::trim_string(_buffer_str);
            // Set name.
            tmp_person.name = _buffer_str;

            // Gender of this person.
            printf("Gender (MALE / FEMALE / TRANSGENDER):\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Judge if buffer string is a valid gender.
            if (lab1::validate_gender(_buffer_str) == -1)
            {
                // Error message.
                printf("Error: Your input of gender is invalid, please retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
                // Minus 1 for retry.
                --i;
                continue;
            }
            else
            {
                // Convert buffer string to required value and stored into the struct Person.
                tmp_person.gender = lab1::validate_gender(_buffer_str);
            }

            // Age of this person.
            printf("Age:\n");
            // Read entire line into buffer string.
            std::getline(std::cin, _buffer_str);
            // Judge if buffer string is a valid age.
            if (!lab1::validate_uint(_buffer_str) || lab1::uint_parser(_buffer_str) == 0)
            {
                // Error message.
                printf("Error: Your input of age is invalid (must be a positive integer), please retry!\n");
#ifndef PRINT_DEBUG_INFO
                // Alert users to see details in debug info.
                printf("Notice: You can define a \"PRINT_DEBUG_INFO\" macro to see debug details!\n");
#endif
                // Minus 1 for retry.
                --i;
                continue;
            }
            else
            {
                // Convert buffer string to required value and stored into the struct Person.
                tmp_person.age = lab1::uint_parser(_buffer_str);
            }

            // Add this person into the Josephus Ring.
            ring.add(tmp_person);
        }
    }
}