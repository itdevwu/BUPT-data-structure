// Copyright 2021 Zhenglong WU.
// All rights reserved.
// This program is written for the course of "Data Structure" in BUPT.

// Standard C++ headers.
#include <iostream>
#include <cstdio>
#include <string>

// Macro for which the program use judging if debug info should be printed or not.
#define PRINT_DEBUG_INFO

// Namespace lab1.
namespace lab1
{
    // Enum for one's gender
    enum Gender
    {
        // 0 for female.
        FEMALE,
        // 1 for male.
        MALE,
        // 2 for transgender.
        TRANSGENDER,
    };

    // A custom implementation for transforming a string to its uppercase form.
    std::string str_to_upper(std::string &target_string)
    {
        // Cache string's size for better performance.
        auto _size = target_string.size();

        // Enumerate each char.
        for (unsigned int i = 0; i < _size; ++i)
        {
            // Modify each char as required and store them by reference.
            auto &_ch = target_string[i];
            if (_ch >= 'a' && _ch <= 'z')
            {
                // Modify each char as required.
                _ch = _ch - 'a' + 'A';
            }
        }

        // Return result.
        return target_string;
    }

    // Remove leading and trailing spaces from input string.
    std::string trim_string(std::string &target_string)
    {
        // Result string.
        std::string _res_str;

        // Start position of the target string.
        auto _str_start = target_string.find_first_not_of(" ");
        // End position of the target string.
        auto _str_end = target_string.find_last_not_of(" ");

        // Judge if the string is empty.
        if (_str_start != std::string::npos)
        {
            _res_str = target_string.substr(_str_start, (_str_end - _str_start + 1));
        }
        else
        {
            // Return empty string.
            _res_str = "";
        }

        // Modify target string by reference.
        target_string = _res_str;

        // Return result.
        return _res_str;
    }

    // Validate if a string is an unsigned integer.
    bool validate_uint(std::string target_string)
    {
        // Result bool.
        auto _result = true;

        // Remove leading and trailing spaces.
        trim_string(target_string);
        // Judge if the first char is 0 or non-numeric char.
        if (target_string[0] == '0' || !isdigit(target_string[0]))
        {
            _result = false;

#ifdef PRINT_DEBUG_INFO
            // Debug info.
            printf("DEBUG INFO: Unsigned integer validation failed! \" %s \" is NOT a valid unsigned integer!\n", target_string.c_str());
#endif
        }
        else
        {
            // Enumerate each char.
            auto _size = target_string.size();
            for (unsigned int i = 1; i < _size; ++i)
            {
                // Judge if it is a digit or not.
                if (!isdigit(target_string[i]))
                {
                    _result = false;

#ifdef PRINT_DEBUG_INFO
                    // Debug info.
                    printf("DEBUG INFO: Unsigned integer validation failed! \" %s \" is NOT a valid unsigned integer!\n", target_string.c_str());
#endif

                    // Break since validation is failed.
                    break;
                }
            }
        }

        // Return.
        return _result;
    }

    // Validate if a string represent a type of gender.
    int validate_gender(std::string target_string)
    {
        // Result indicator, -1 is fail and 0~2 represent diffrent gender.
        auto _result = -1;

        // Remove leading and trailing spaces.
        trim_string(target_string);
        // Convert target string to its uppercase form.
        target_string = str_to_upper(target_string);

        // Judge gender.
        if (target_string == "MALE")
        {
            // Male.
            _result = MALE;
        }
        else if (target_string == "FEMALE")
        {
            // Female.
            _result = FEMALE;
        }
        else if (target_string == "TRANSGENDER")
        {
            // Transgender.
            _result = TRANSGENDER;
        }
        else
        {
            // Convertion failed.
            _result = -1;
#ifdef PRINT_DEBUG_INFO
            printf("DEBUG INFO: Gender validation failed! \" %s \" is NOT a valid gender!\n", target_string.c_str());
#endif
        }

        // Return.
        return _result;
    }

    // Parse string into unsigned integer.
    unsigned int uint_parser(std::string target_string)
    {
        // Result uint.
        unsigned int _result = 0;

        // Judge if this string is convertable.
        if (validate_uint(target_string) == false)
        {
#ifdef PRINT_DEBUG_INFO
            printf("DEBUG INFO: Failed to parser string \" %s \" to unsigned integer!\n", target_string.c_str());
#endif
        }
        else
        {
            auto _size = target_string.size();
            for (unsigned int i = 0; i < _size; ++i)
            {
                // Fetch the (i+1)-th char.
                auto _ch = target_string[i];
                // Add result number.
                _result = _result * 10 + _ch - '0';
            }
        }

        // Return.
        return _result;
    }

    // Convert cardinal number to ordinal number.
    std::string cardinal_to_ordinal(unsigned int number)
    {
        // Result string.
        std::string _result = std::to_string(number);

        // In English, numbers end with 11, 12 and 13 have different ordinal form.
        // Handle numbers end with 11, 12 and 13.
        switch (number % 100)
        {
            // 11, 111, 211, etc.
        case 11:
            _result.append("th");
            break;
            // 12, 112, 312, etc.
        case 12:
            _result.append("th");
            break;
            // 13, 113, 11113, etc.
        case 13:
            _result.append("th");
            break;

        // Normal numbers.
        // Handle xx-first, xx-second, xx-third.
        default:
            switch (number % 10)
            {
                // 1, 21, 151, etc.
            case 1:
                // First,
                _result.append("st");
                break;
                // 2, 22, 152, etc.
            case 2:
                _result.append("nd");
                break;
                // 3, 23, 153, etc.
            case 3:
                _result.append("rd");
                break;
            default:
                _result.append("th");
                break;
            }
            break;
        }

        // Return.
        return _result;
    }
}