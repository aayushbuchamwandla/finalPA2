#include "movies.h"
#include <iostream>
#include <vector>
#include <algorithm>

bool compareMoviesForOutput(const Movie& m1, const Movie& m2) {
    if (m1.rating != m2.rating) {
        return m1.rating > m2.rating;
    } else {
        return m1.name < m2.name;
    }
}

void PrintByRating(std::vector<Movie>& m) {
    std::sort(m.begin(), m.end(), compareMoviesForOutput);

    for (const auto& movie : m) {
        std::cout << movie.name << ", " << movie.rating << std::endl;
    }
}
