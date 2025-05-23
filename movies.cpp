#include "movies.h"
#include <algorithm>

bool compareByRating(const Movie& a, const Movie& b) {
    if (a.rating != b.rating) {
        return a.rating > b.rating;
    }
    return a.name < b.name;
}

void findMoviesByPrefix(const std::vector<Movie>& movies, const std::string& prefix, 
                       std::vector<Movie>& results) {
    results.clear();
    
    if (prefix.empty()) {
        return;
    }
    
    // Find lower bound
    auto low = std::lower_bound(movies.begin(), movies.end(), 
                              Movie(prefix, 0),
                              [](const Movie& m, const Movie& value) {
                                  return m.name < value.name;
                              });
    
    // Find upper bound (prefix incremented)
    std::string upperPrefix = prefix;
    upperPrefix.back()++;
    auto high = std::lower_bound(movies.begin(), movies.end(), 
                               Movie(upperPrefix, 0),
                               [](const Movie& m, const Movie& value) {
                                   return m.name < value.name;
                               });
    
    // Copy matches
    results.assign(low, high);
    
    // Sort by rating
    std::sort(results.begin(), results.end(), compareByRating);
}
