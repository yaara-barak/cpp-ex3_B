#include "NumberWithUnits.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
using namespace std;
const double EPS=0.0001;


namespace ariel{
  
			map<string, map<string, double> > NumberWithUnits::units;
      
      NumberWithUnits::NumberWithUnits(const double u,const string &t){
        if (units.count(t)==0|| t.empty()){
          throw runtime_error("wrong unit type");
        }
        this->unit=u;
        this->type=t;
      }
			
      void NumberWithUnits::read_units(ifstream &text){
          if (text)
          {
            double num1=0;
            double num2=0;
            string from, to, op;
            while(text >> num1 >> from >> op >> num2 >> to){
              //add to the map the units
              if (units.count(from)==0){units.insert({from, map<string, double>{}});}
              if (units.count(to)==0){units.insert({to, map<string, double>{}});}
              units.at(from).insert({to,num2});
              units.at(to).insert({from,1/num2});
              for(auto const &map : units.at(from)){
                units.at(map.first).insert({to,1/(map.second * units.at(to).at(from))});
                units.at(to).insert({map.first, map.second * units.at(to).at(from)});
              }
              for(auto const &map : units.at(to)){
                units.at(map.first).insert({from,1/(map.second * units.at(from).at(to))});
                units.at(from).insert({map.first, map.second * units.at(from).at(to)});
              }
            }
            text.close();
          }
          else {
             throw invalid_argument("no file");
          }
      }

      double NumberWithUnits::convert (const string &from, const string &to, double num){
        if(units.at(from).count(to)==0){
          throw invalid_argument{"illeagal operation"};
        }
        return num*units.at(from).at(to);   
      }

			NumberWithUnits operator + (const NumberWithUnits &num1, const NumberWithUnits &num2){
        double n2= NumberWithUnits::convert(num2.type, num1.type, num2.unit);
        double sum= num1.unit+n2;
        return NumberWithUnits(sum,num1.type);
      }
      NumberWithUnits &NumberWithUnits::operator += (const NumberWithUnits &num){
        this->unit += convert(num.type, this->type, num.unit);
        return *this;
      }
			NumberWithUnits operator + (const NumberWithUnits& num1){
        return num1;
      }
			NumberWithUnits operator - (const NumberWithUnits &num1, const NumberWithUnits &num2){
        double n2= NumberWithUnits::convert(num2.type, num1.type, num2.unit);
        double sum= num1.unit-n2;
        return NumberWithUnits(sum,num1.type);
      }
			NumberWithUnits &NumberWithUnits::operator -= (const NumberWithUnits &num){
        this->unit -= convert(num.type, this->type, num.unit);
        return *this;
      }
			NumberWithUnits operator - (const NumberWithUnits& num1){
        return NumberWithUnits(num1.unit*(-1),num1.type);
      }
			NumberWithUnits operator* (const double mult, const NumberWithUnits &num){
        
        return NumberWithUnits(num.unit*mult,num.type);
      }
      NumberWithUnits operator* (const NumberWithUnits &num, const double mult){
        return NumberWithUnits(num.unit*mult,num.type);
      }

			bool NumberWithUnits::operator>(const NumberWithUnits &num) const{
          if (type!=num.type && units.at(type).count(num.type)==0){
            throw invalid_argument("illeagal operation");
          }
          if (type==num.type){
            return (unit>num.unit);
          }
           return convert(type,num.type,unit)>num.unit;
      }
      bool NumberWithUnits::operator>=(const NumberWithUnits &num) const{
          if (type!=num.type && units.at(type).count(num.type)==0){
            throw invalid_argument("illeagal operation");
          }
          if (type==num.type){
            return (unit>num.unit||abs(unit-num.unit)<EPS);
          }
          return convert(type,num.type,unit)>num.unit||abs(convert(type,num.type,unit)-num.unit)<EPS;
      }
      bool NumberWithUnits::operator<(const NumberWithUnits &num) const{
        if (type!=num.type && units.at(type).count(num.type)==0){
            throw invalid_argument("illeagal operation");
          }
          if (type==num.type){
            return (unit<num.unit);
          }
          return convert(type,num.type,unit)<num.unit;
      }
      bool NumberWithUnits::operator<=(const NumberWithUnits &num) const{
        if (type!=num.type && units.at(type).count(num.type)==0){
            throw invalid_argument("illeagal operation");
          }
          if (type==num.type){
            return (unit<=num.unit);
          }
          return convert(type,num.type,unit)<=num.unit;
      }
			bool NumberWithUnits::operator==(const NumberWithUnits &num) const{
        
        if (type!=num.type && units.at(type).count(num.type)==0){
            throw invalid_argument("illeagal operation");
        }
        if (type==num.type){
            return abs(unit-num.unit)<EPS;
        }
        double sum= convert(type,num.type,unit);
        return abs(sum-num.unit)<EPS;  
      }
      bool NumberWithUnits::operator!=(const NumberWithUnits &num) const{
          return !this->operator==(num);
      }

			NumberWithUnits& NumberWithUnits::operator++(){
        unit++;
        return *this;
      }         
      NumberWithUnits NumberWithUnits::operator++(int){
        double u=unit;
        unit++;
        return NumberWithUnits(u,type);
      }
      NumberWithUnits& NumberWithUnits::operator--(){
        unit--;
        return *this;
      }       
      NumberWithUnits NumberWithUnits::operator--(int){
        double u=unit;
        unit--;
        return NumberWithUnits(u,type);
      }

			ostream& operator<<(ostream &os, const NumberWithUnits &num){
        os << num.unit << "[" << num.type << "]";
        return os;
      }
     
      istream& operator>>(istream &is, NumberWithUnits &num){
        char start=' ';
        char end=' ';
        string type;
        double unit=0;
        is>>skipws >>unit>>start>>type;
        if (type.at(type.length()-1)==']'){
          type=type.substr(0,type.length()-1);
        }
        else{
            is >> skipws >> end;
        }
        if(type.empty()){
          throw invalid_argument{"illeagal operation"};
        }
        num.type=type;
        num.unit=unit;
        if (NumberWithUnits::units.count(num.type)==0){
          throw invalid_argument{"illeagal operation"};
        }
        return is;
      }

};