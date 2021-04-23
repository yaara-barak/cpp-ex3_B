#pragma once
#include <iostream>
#include <string>
#include <map>

namespace ariel{
		class NumberWithUnits{
	
		private:
			double unit;
			std::string type;
			static std::map<std::string, std::map<std::string, double> > units;


		public:
			NumberWithUnits(double, const std::string&);
			static void read_units(std:: ifstream&);
			static double convert(const std::string&,const std::string&,double);
			
			friend NumberWithUnits operator + (const NumberWithUnits &num1, const NumberWithUnits &num2);
			NumberWithUnits &operator += (const NumberWithUnits &num);
			friend NumberWithUnits operator + (const NumberWithUnits& num1);
			friend NumberWithUnits operator - (const NumberWithUnits &num1, const NumberWithUnits &num2);
			NumberWithUnits &operator -= (const NumberWithUnits &num);
			friend NumberWithUnits operator - (const NumberWithUnits& num1);
			friend NumberWithUnits operator* (const double mult, const NumberWithUnits &num);
        	friend NumberWithUnits operator* (const NumberWithUnits &num, const double mult);

			bool operator>(const NumberWithUnits &num) const;
        	bool operator>=(const NumberWithUnits &num) const;
        	bool operator<(const NumberWithUnits &num) const;
        	bool operator<=(const NumberWithUnits &num) const;
			bool operator==(const NumberWithUnits &num) const;
        	bool operator!=(const NumberWithUnits &num) const;

			NumberWithUnits& operator++();         
        	NumberWithUnits operator++(int); 
        	NumberWithUnits& operator--();         
        	NumberWithUnits operator--(int); 

			friend std::ostream& operator<<(std::ostream &os, const NumberWithUnits &num);
        	friend std::istream& operator>>(std::istream &is, NumberWithUnits &num);

	};
}