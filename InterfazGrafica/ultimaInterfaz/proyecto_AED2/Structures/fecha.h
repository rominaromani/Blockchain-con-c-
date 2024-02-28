#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <sstream> // incluir para istringstream

using namespace std;

class Fecha {
private:
    int day;
    int month;
    int year;

public:
    Fecha() {
        day = 0;
        month = 0;
        year = 0;
    }

    Fecha(int day, int month, int year) {
        this->day = day;
        this->month = month;
        this->year = year;
    }

    Fecha(string date) {
        istringstream iss(date); // usar istringstream para leer la cadena
        char delimiter; // variable para almacenar los caracteres separadores
        iss >> day >> delimiter >> month >> delimiter >> year; // extraer valores numéricos
    }

    void setFecha(string date) {
        istringstream iss(date); // usar istringstream para leer la cadena
        char delimiter; // variable para almacenar los caracteres separadores
        iss >> day >> delimiter >> month >> delimiter >> year; // extraer valores numéricos
    }

    string getString() {
        string result;
        if (day < 10) { // agregar cero adelante si el día tiene menos de 2 dígitos
            result += "0";
        }
        result += to_string(day) + "/";
        if (month < 10) { // agregar cero adelante si el mes tiene menos de 2 dígitos
            result += "0";
        }
        result += to_string(month) + "/";
        result += to_string(year);
        return result;
    }

    ~Fecha() {}

    bool operator>(const Fecha& other) const {
        if (year > other.year)
            return true;
        else if (year == other.year) {
            if (month > other.month)
                return true;
            else if (month == other.month) {
                if (day > other.day)
                    return true;
            }
        }
        return false;
    }

    bool operator<(const Fecha& other) const {
        if (year < other.year)
            return true;
        else if (year == other.year) {
            if (month < other.month)
                return true;
            else if (month == other.month) {
                if (day < other.day)
                    return true;
            }
        }
        return false;
    }

    bool operator==(const Fecha& other) const {
        return (day == other.day && month == other.month && year == other.year);
    }

    bool operator<=(const Fecha& other) const {
        return (*this < other || *this == other);
    }

    bool operator>=(const Fecha& other) const {
        return (*this > other || *this == other);
    }
};

#endif