// Spring '25
// Instructor: Diba Mirza
// Student name: Ashwin Kannan, Aayush Buchamwandla
#include "movies.h"

using namespace std;

bool compareByRating(const Movie& a, const Movie& b) {
    if (a.rating != b.rating) {
        return a.rating > b.rating;
    }
    return a.name < b.name;
}

void findMoviesByPrefix(const vector<Movie>& movies, const string& prefix, 
                       vector<Movie>& results) {
    results.clear();
    
    if (prefix.empty()) {
        return;
    }
    
    auto low = lower_bound(movies.begin(), movies.end(), 
                         Movie(prefix, 0),
                         [](const Movie& m, const Movie& value) {
                             return m.name < value.name;
                         });
    
    string upperPrefix = prefix;
    upperPrefix.back()++;
    auto high = lower_bound(movies.begin(), movies.end(), 
                          Movie(upperPrefix, 0),
                          [](const Movie& m, const Movie& value) {
                              return m.name < value.name;
                          });
    
    results.assign(low, high);
    sort(results.begin(), results.end(), compareByRating);
}
