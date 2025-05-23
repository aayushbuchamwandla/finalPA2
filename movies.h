#ifndef MOVIES_H
#define MOVIES_H

#include <string>
#include <vector>
#include <algorithm>

struct Movie {
    std::string name;
    double rating;

    Movie(const std::string& n, double r) : name(n), rating(r) {}
    Movie() : name(""), rating(0.0) {}

    bool operator<(const Movie& other) const {
        return name < other.name;
    }
};

void PrintByRating(std::vector<Movie>& m);

#endif
