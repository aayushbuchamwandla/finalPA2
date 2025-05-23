// Spring '25
// Instructor: Diba Mirza
// Student name: Ashwin Kannan
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);

    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    string line, movieName;
    double movieRating;

    vector<Movie> movies2;
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        movies2.push_back(Movie(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
        sort(movies2.begin(), movies2.end(), [](const Movie& a, const Movie& b) {
            return a.name < b.name;
        });

        for (const Movie& m : movies2) {
            cout << m.name << ", " << m.rating << endl;
        }
        return 0;
    }

    ifstream prefixFile (argv[2]);
    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    sort(movies2.begin(), movies2.end(), [](const Movie& a, const Movie& b) {
        return a.name < b.name;
    });

    vector<string> finalLines;

    for (const string& prefix : prefixes) {
        auto it = lower_bound(movies2.begin(), movies2.end(), prefix, [](const Movie& a, const string& p){
            return a.name < p;
        });

        vector<Movie> shortened;
        while (it != movies2.end() && it->name.rfind(prefix, 0) == 0) {
            shortened.push_back(*it);
            ++it;
        }

        if (!shortened.empty()) {
            PrintByRating(shortened);
            stringstream ss;
            ss << "Best movie with prefix " << prefix << " is: " << shortened[0].name << " with rating " << std::fixed << std::setprecision(1) << shortened[0].rating;
            finalLines.push_back(ss.str());
        } else {
            cout << "No movies found with prefix " << prefix << endl;
        }
        cout << endl;
    }

    for (const string& s : finalLines) {
        cout << s << endl;
    }

    return 0;
}

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
