#ifndef MOVIES_H
#define MOVIES_H

#include <iostream>
#include <vector>
#include <string>

struct Movie {
    std::string name;
    double rating;

    Movie(std::string name, double rating) : name(name), rating(rating) {}
    
    // For sorting by name
    bool operator<(const Movie& other) const {
        return name < other.name;
    }
};

// For sorting by rating then name
bool compareByRating(const Movie& a, const Movie& b);

// Binary search based prefix search
void findMoviesByPrefix(const std::vector<Movie>& movies, const std::string& prefix, 
                       std::vector<Movie>& results);

#endif // MOVIES_H
