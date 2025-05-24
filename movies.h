#ifndef MOVIES_H
#define MOVIES_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Movie {
    string name;
    double rating;

    Movie(string name, double rating) : name(name), rating(rating) {}
    
    bool operator<(const Movie& other) const {
        return name < other.name;
    }
};

bool compareByRating(const Movie& a, const Movie& b);
void findMoviesByPrefix(const vector<Movie>& movies, const string& prefix, vector<Movie>& results);

#endif
