// Spring '25
// Instructor: Diba Mirza
// Student name: Ashwin Kannan

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

// Helper: Find the lower bound index of the prefix
vector<Movie>::iterator findPrefixStart(vector<Movie>& movies, const string& prefix) {
    return lower_bound(movies.begin(), movies.end(), prefix, [](const Movie& m, const string& p) {
        return m.name < p;
    });
}

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
    map<string,double> movies;
    vector<Movie> movies2;

    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        movies[movieName] = movieRating;
        movies2.emplace_back(movieName, movieRating);
    }
    movieFile.close();

    if (argc == 2){
        for (auto& [name, rating] : movies) {
            cout << name << ", " << rating << endl;
        }
        return 0;
    }

    ifstream prefixFile (argv[2]);
    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) prefixes.push_back(line);
    }

    // Efficient step: sort once by name
    sort(movies2.begin(), movies2.end(), [](const Movie& a, const Movie& b) {
        return a.name < b.name;
    });

    vector<string> finalLines;

    for (const string& prefix : prefixes) {
        auto it = findPrefixStart(movies2, prefix);
        vector<Movie> matches;

        while (it != movies2.end() && it->name.compare(0, prefix.size(), prefix) == 0) {
            matches.push_back(*it);
            ++it;
        }

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            sort(matches.begin(), matches.end(), [](const Movie& a, const Movie& b) {
                if (a.rating != b.rating) return a.rating > b.rating;
                return a.name < b.name;
            });

            stringstream ss;
            ss << "Best movie with prefix " << prefix << " is: " << matches[0].name
               << " with rating " << fixed << setprecision(1) << matches[0].rating;
            finalLines.push_back(ss.str());
        }
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
