#include "airline.h"

Airline::Airline(string code, string name, string callSign, string country){
    this->code = std::move(code);
    this->name = std::move(name);
    this->callSign = std::move(callSign);
    this->country = std::move(country);
}

Airline::Airline(string code){
    this->code = std::move(code);
}

string Airline::getCode() const {return this->code;}
string Airline::getName(){return this->name;}
string Airline::getCountry(){return this->country;}



