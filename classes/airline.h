//
// Created by Utilizador on 03/12/2023.
//
#ifndef AIRBUSMANAGEMENTSYSTEM_AIRLINE_H
#define AIRBUSMANAGEMENTSYSTEM_AIRLINE_H
#include <iostream>
#include <unordered_set>
using namespace std;

/**
 * @file
 * @brief Contains the Airline class that represents airline information and its hash structure.
 */

/**
 * @class Airline
 * @brief Represents information about an airline.
 */
class Airline {
public:
    /**
     * @brief Constructor for Airline class that takes only the airline code.
     * @param code The airline code.
     */
    explicit Airline(string code);

    /**
     * @brief Constructor for Airline class that takes all airline information.
     * @param code The airline code.
     * @param name The name of the airline.
     * @param callSign The call sign of the airline.
     * @param country The country of the airline.
     */
    Airline(string code, string name, string callSign, string country);

    /**
     * @struct AirlineHash
     * @brief Structure defining the hash function and equality operator for Airline objects.
     */
    struct AirlineHash {
        /**
         * @brief Hash function for an Airline object.
         * @param b The Airline object to hash.
         * @return The hash value for the Airline object.
         */
        int operator()(const Airline &b) const {
            string c = b.getCode();
            int v = 0;
            int primeMultiplier = 131; // Change to a larger prime number
            for (const char &i : c)
                v = primeMultiplier * v + i;
            return v;
        }

        /**
         * @brief Equality operator for comparing two Airline objects.
         * @param b1 The first Airline object.
         * @param b2 The second Airline object.
         * @return True if the airline codes of both objects are equal, otherwise false.
         */
        bool operator()(const Airline &b1, const Airline &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

    /**
     * @brief Gets the airline code.
     * @return The airline code.
     */
    string getCode() const;

    /**
     * @brief Gets the name of the airline.
     * @return The name of the airline.
     */
    string getName();

    /**
     * @brief Gets the country of the airline.
     * @return The country of the airline.
     */
    string getCountry();

    /**
     * @brief Defines an unordered set of Airline objects using the custom hash and equality function.
     */
    typedef unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> AirlineH;

private:
    string code;     /**< The airline code. */
    string name;     /**< The name of the airline. */
    string callSign; /**< The call sign of the airline. */
    string country;  /**< The country of the airline. */
};

#endif //AIRBUSMANAGEMENTSYSTEM_AIRLINE_H
