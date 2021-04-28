#include "NumberWithUnits.hpp"
#include <iostream>
#include <map>
#include <fstream>
#include <string>
using namespace std;

//https://en.cppreference.com/w/cpp/language/operator_precedence
//https://riptutorial.com/cplusplus/example/17316/auto--const--and-references

namespace ariel{
    
     map<string,map<string, double>> convertMap;

    NumberWithUnits:: NumberWithUnits(double num, const string &unit) : num(num), unit(unit){
        convertMap.at(unit);
    }
    
    double testUnit(const string& a, const string& b, double num){
        if(a==b){
            return num;
        }
        if(convertMap.at(a).at(b) == 0){
            throw invalid_argument{"unit don't convert"};
        }
        return num*convertMap.at(a).at(b);
    };

    void NumberWithUnits::read_units(std::ifstream &units_file){
         string from_unit, to_unit, equal;
        double num1=0;
        double num2=0;

        while(units_file >> num1 >> from_unit >> equal >> num2 >> to_unit){
            convertMap[from_unit][to_unit] = num2;
            convertMap[to_unit][from_unit] = num1/num2;

            for(const auto& pair : convertMap[to_unit]){
                double a = convertMap[from_unit][to_unit]*pair.second;
                convertMap[from_unit][pair.first] = a;
                convertMap[pair.first][from_unit] = 1/a;
            }

            for(const auto& pair : convertMap[from_unit]){
                double a = convertMap[to_unit][from_unit]*pair.second;
                convertMap[to_unit][pair.first] = a;
                convertMap[pair.first][to_unit] = 1/a;
            }
        }
        
    }
    
    //overloading in and out operators
    ostream &operator<<(ostream &os, const NumberWithUnits &numUnit){
        return os<< numUnit.num << "[" << numUnit.unit << "]";  
    }

    std::istream &operator>>(std::istream &is, NumberWithUnits &numUnit){
        string type;
        double value = 0; 
        char c=']';

        is >> value >> c;

        while (c!=']')
        {
            if(c !='['){
                type.insert(type.end(),c);
            }
            is>>c;
        }
        if(convertMap.at(type).empty()){
            throw invalid_argument{"types can't match"};
        }
        
        numUnit.num = value;
        numUnit.unit = type;
        return is;
    }

    //overloading arithmetic operators
    NumberWithUnits operator+(const NumberWithUnits &numUnit){
        return NumberWithUnits(numUnit.num, numUnit.unit);
    }
    NumberWithUnits operator+(const NumberWithUnits &numUnit1, const NumberWithUnits &numUnit2){
        return NumberWithUnits(numUnit1.num + testUnit(numUnit2.unit, numUnit1.unit, numUnit2.num),numUnit1.unit);
    }
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &numUnit) {
        this->num+= testUnit(numUnit.unit, this->unit, numUnit.num);
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator++(){
        ++(this->num);
        return *this;
    }
    NumberWithUnits NumberWithUnits::operator++(int){

        return NumberWithUnits(this->num++, this->unit);
    }
    NumberWithUnits& NumberWithUnits::operator--(){
        --(this->num);
        return *this; 
    }
    NumberWithUnits NumberWithUnits::operator--(int){
        return NumberWithUnits(this->num--, this->unit);
    }

    NumberWithUnits operator-(const NumberWithUnits &numUnit){
        return NumberWithUnits(numUnit.num*(-1), numUnit.unit);
    }
    NumberWithUnits operator-(const NumberWithUnits &numUnit1, const NumberWithUnits &numUnit2){
        return NumberWithUnits(numUnit1.num - testUnit(numUnit2.unit, numUnit1.unit, numUnit2.num),numUnit1.unit);
    }
    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &numUnit){
        this->num-= testUnit(numUnit.unit, this->unit, numUnit.num);
        return *this;
    }

    NumberWithUnits operator*(const NumberWithUnits &numUnit, const double num){
        return NumberWithUnits(numUnit.num*num, numUnit.unit);
    }
    NumberWithUnits operator*(const double num, const NumberWithUnits &numUnit){
        return NumberWithUnits(num*numUnit.num, numUnit.unit);
    }

    //Comparison operators
    bool NumberWithUnits::operator>(const NumberWithUnits &numUnit) const{
        double convert = testUnit(numUnit.unit, this->unit, numUnit.num);
        return this->num > convert;
    }
    bool NumberWithUnits::operator>=(const NumberWithUnits &numUnit) const{
        double convert = testUnit(numUnit.unit, this->unit, numUnit.num);
        return this->num >= convert;
    }
    bool NumberWithUnits::operator<(const NumberWithUnits &numUnit) const{
        double convert = testUnit(numUnit.unit, this->unit, numUnit.num);
        return this->num < convert;
    }
    bool NumberWithUnits::operator<=(const NumberWithUnits &numUnit) const{
       double convert = testUnit(numUnit.unit, this->unit, numUnit.num);
        return this->num <= convert;
    }
    bool NumberWithUnits::operator==(const NumberWithUnits &numUnit) const{
        double convert = testUnit(numUnit.unit, this->unit, numUnit.num);
        return (abs(this->num - convert) <= 0.001);
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits &numUnit) const{
        return !(*this==numUnit);
    }   


};
