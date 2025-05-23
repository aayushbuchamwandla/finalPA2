#include <algorithm>
#include <vector>

struct Movie {
    string name;
    double rating;

    Movie(string name, double rating) : name(name), rating(rating) {}
    
    // For sorting by name
    bool operator<(const Movie& other) const {
        return name < other.name;
    }
};

// For sorting by rating then name
bool compareByRating(const Movie& a, const Movie& b);

// Binary search based prefix search
void findMoviesByPrefix(const vector<Movie>& movies, const string& prefix, 
                       vector<Movie>& results);
