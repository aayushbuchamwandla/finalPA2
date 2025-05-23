#include <iostream>
#include "movies.h"
using namespace std;

bool operations(Movie m1, Movie m2) {
    if (m1.rating!=m2.rating) {
        return m1.rating>m2.rating;
    }
    else {
        return m1.name<m2.name;
    }
}

void PrintByRating(vector<Movie> &m) {
    sort(m.begin(),m.end(),operations);
    for (int i=0; i<m.size(); i++) {
        cout<<m.at(i).name<<", "<<m.at(i).rating<<endl;
    }
}
