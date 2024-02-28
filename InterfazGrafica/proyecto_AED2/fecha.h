#include <iostream>

using namespace std;

class Fecha{
private:
    int day;
    int month;
    int year;
    
public:
    Fecha(){
        day = 0;
        month = 0;
        year = 0;
    }
    Fecha(int day, int month, int year){
        this->day = day;
        this->month = month;
        this->year = year;
    }

    Fecha(string date){
        string day = date.substr(0,2);
        string month = date.substr(3,2);
        string year = date.substr(6,4);
        this->day = stoi(day);
        this->month = stoi(month);
        this->year = stoi(year);
    }

    string getString(){
        string result;
        result = to_string(day) + "/" + to_string(month) + "/" + to_string(year); 
        return result;
    }

    ~Fecha(){
    }

    bool operator>(const Fecha& other) const{
        if(year > other.year)
            return true;
        else if(year == other.year){
            if(month > other.month)
                return true;
            else if(month == other.month){
                if(day > other.day)
                    return true;
            }
        }
        return false;
    }

    bool operator<(const Fecha& other) const{
        if(year < other.year)
            return true;
        else if(year == other.year){
            if(month < other.month)
                return true;
            else if(month == other.month){
                if(day < other.day)
                    return true;
            }
        }
        return false;
    }

    bool operator==(const Fecha& other) const{
        return (day == other.day && month == other.month && year == other.year);
    }
};