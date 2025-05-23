// Spring '25
// Instructor: Diba Mirza
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "movies.h"
using namespace std;

bool parseLine(string &line, string &movieName, double &movieRating) {
    size_t commaIndex = line.find_last_of(',');
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex + 1));
    if (!movieName.empty() && movieName.front() == '"') {
        movieName = movieName.substr(1, movieName.size() - 2);
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " movieFilename [prefixFilename]" << endl;
        return 1;
    }

    ifstream movieFile(argv[1]);
    if (!movieFile.is_open()) {
        cerr << "Could not open file " << argv[1] << endl;
        return 1;
    }

    vector<Movie> movies;
    string line, name;
    double rating;
    while (getline(movieFile, line)) {
        if (parseLine(line, name, rating)) {
            movies.emplace_back(name, rating);
        }
    }
    movieFile.close();

    sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        return a.name < b.name;
    });

    if (argc == 2) {
        for (const auto& m : movies) {
            cout << m.name << ", " << m.rating << endl;
        }
        return 0;
    }

    ifstream prefixFile(argv[2]);
    if (!prefixFile.is_open()) {
        cerr << "Could not open prefix file " << argv[2] << endl;
        return 1;
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) prefixes.push_back(line);
    }

    vector<string> finalLines;
    for (const string& prefix : prefixes) {
        auto it = lower_bound(movies.begin(), movies.end(), prefix, [](const Movie& m, const string& p) {
            return m.name < p;
        });

        vector<Movie> matches;
        while (it != movies.end() && it->name.compare(0, prefix.length(), prefix) == 0) {
            matches.push_back(*it++);
        }

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            sort(matches.begin(), matches.end(), operations);
            PrintByRating(matches);
            stringstream ss;
            ss << "Best movie with prefix " << prefix << " is: " 
               << matches[0].name << " with rating " 
               << fixed << setprecision(1) << matches[0].rating;
            finalLines.push_back(ss.str());
        }

        cout << endl;
    }

    for (const string& result : finalLines) {
        cout << result << endl;
    }

    return 0;
}

