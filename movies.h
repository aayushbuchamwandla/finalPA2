#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Movie {
    string name;
    double rating;

    Movie(string name, double rating) : name(name), rating(rating) {}
};

void PrintByRating(vector<Movie> &m); 
bool operations(Movie m1, Movie m2);
