//
// Created by Utilizador on 03/12/2023.
//

#ifndef AIRBUSMANAGEMENTSYSTEM_AIRPORT_H
#define AIRBUSMANAGEMENTSYSTEM_AIRPORT_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

/**
 * @file
 * @brief Contains the Airport class that represents airport information and related hash structures.
 */

/**
 * @class Airport
 * @brief Represents information about an airport.
 */
class Airport {
public:
    /**
     * @brief Constructor for Airport class that takes only the airport code.
     * @param code The airport code.
     */
    explicit Airport(string code);

    /**
     * @brief Constructor for Airport class that takes all airport information.
     * @param code The airport code.
     * @param name The name of the airport.
     * @param city The city where the airport is located.
     * @param country The country where the airport is located.
     * @param latitude The latitude coordinate of the airport.
     * @param longitude The longitude coordinate of the airport.
     */
    Airport(string code, string name, string city, string country, double latitude, double longitude);

    /**
     * @brief Gets the airport code.
     * @return The airport code.
     */
    string getCode() const;

    /**
     * @brief Gets the name of the airport.
     * @return The name of the airport.
     */
    string getName() const;

    /**
     * @brief Gets the city where the airport is located.
     * @return The city where the airport is located.
     */
    string getCity() const;

    /**
     * @brief Gets the country where the airport is located.
     * @return The country where the airport is located.
     */
    string getCountry() const;

    /**
     * @brief Gets the latitude coordinate of the airport.
     * @return The latitude coordinate of the airport.
     */
    double getLatitude() const;

    /**
     * @brief Gets the longitude coordinate of the airport.
     * @return The longitude coordinate of the airport.
     */
    double getLongitude() const;

    /**
     * @struct AirportHash
     * @brief Structure defining the hash function and equality operator for Airport objects.
     */
    struct AirportHash {
        /**
         * @brief Hash function for an Airport object.
         * @param b The Airport object to hash.
         * @return The hash value for the Airport object based on its code.
         */
        int operator()(const Airport &b) const {
            string c = b.getCode();
            int v = 0;
            for (char i : c) v = 131 * v + i;
            return v;
        }

        /**
         * @brief Equality operator for comparing two Airport objects.
         * @param b1 The first Airport object.
         * @param b2 The second Airport object.
         * @return True if the airport codes of both objects are equal, otherwise false.
         */
        bool operator()(const Airport &b1, const Airport &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

    /**
     * @struct CityHash
     * @brief Structure defining the hash function and equality operator for City information (pair of city name and city code).
     */
    struct CityHash {
        /**
         * @brief Hash function for a pair of city name and city code.
         * @param b The pair representing city name and city code.
         * @return The hash value for the pair based on city code.
         */
        int operator()(const pair<string,string> &b) const {
            string cityCode = b.second;
            int v = 0;
            for (char i : cityCode) v = 131 * v + i;
            return v;
        }

        /**
         * @brief Equality operator for comparing two pairs of city name and city code.
         * @param b1 The first pair of city name and city code.
         * @param b2 The second pair of city name and city code.
         * @return True if both pairs have the same city name and city code, otherwise false.
         */
        bool operator()(const pair<string,string> &b1, const pair<string,string> &b2) const {
            return b1.first == b2.first && b1.second== b2.second;
        }
    };

    /**
     * @brief Defines an unordered set of pairs representing city name and city code using the custom hash and equality function.
     */
    typedef unordered_set<pair<string, string>, Airport::CityHash, Airport::CityHash> CityH2;

    /**
     * @brief Defines an unordered map of pairs representing city name and city code to a vector of strings (possibly airport codes) using the custom hash and equality function.
     */
    typedef unordered_map<pair<string, string>, vector<string>, Airport::CityHash, Airport::CityHash> CityH;

    /**
     * @brief Defines an unordered set of Airport objects using the custom hash and equality function.
     */
    typedef unordered_set<Airport, Airport::AirportHash, Airport::AirportHash> AirportH;

private:
    string code;        /**< The airport code. */
    string name;        /**< The name of the airport. */
    string city;        /**< The city where the airport is located. */
    string country;     /**< The country where the airport is located. */
    double latitude{};  /**< The latitude coordinate of the airport. */
    double longitude{}; /**< The longitude coordinate of the airport. */
};




#endif //AIRBUSMANAGEMENTSYSTEM_AIRPORT_H
