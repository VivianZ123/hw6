#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
using namespace std;

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
    std::vector<std::size_t> char_values(k.length());
    for (std::size_t i = 0; i < k.length(); i++) {
        char_values[i] = letterDigitToNumber(k[i]);
    }

    unsigned long long int base36_segments[5] = {0, 0, 0, 0, 0};
    int char_index = k.length() - 1;  
    int segment_index = 4;  
    const unsigned long long pow36[6] = {
        1, 36, 1296, 46656, 1679616, 60466176
    };

    while (char_index >= 0) {
        unsigned long long int segment_value = 0;
        for (int pos = 0; pos < 6 && char_index >= 0; pos++) {
            segment_value += char_values[char_index--] * pow36[pos];
        }
        base36_segments[segment_index--] = segment_value;
    }

    unsigned long long int hash_value = rValues[0] * base36_segments[0];
    for (int i = 1; i < 5; i++) {
        hash_value += rValues[i] * base36_segments[i];
    }
    return hash_value;
  }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
      if (isdigit(letter)) {
        return letter - '0' + 26; 
    } 
    else if (isalpha(letter)) {
        return tolower(letter) - 'a';
    }
    return 0; 
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
