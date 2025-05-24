#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "movies.h"
using namespace std;

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << endl;
        return(1);
    }

    ifstream movieFile(argv[1]);
    if (movieFile.fail()) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return(1);
    }

    vector<Movie> movies;
    string line, movieName;
    double movieRating;

    // Read and store all movies
    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
        movies.push_back(movieName, movieRating);
    }
    movieFile.close();

    // Sort movies by name once
    sort(movies.begin(), movies.end());

    if (argc == 2) {
        // Print all movies in alphabetical order
        for (const auto& movie : movies) {
            cout << movie.name << ", " << movie.rating << std::endl;
        }
        return 0;
    }

    ifstream prefixFile(argv[2]);
    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2] << std::endl;
        return(1);
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();

    vector<Movie> matches;
    vector<std::string> bestMovieLines;

    for (const auto& prefix : prefixes) {
        findMoviesByPrefix(movies, prefix, matches);

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << std::endl;
        } else {
            // Print all matches
            for (const auto& movie : matches) {
                cout << movie.name << ", " << movie.rating << endl;
            }
            cout << std::endl;

            // Store best movie line
            stringstream ss;
            ss << "Best movie with prefix " << prefix << " is: " 
               << matches[0].name << " with rating " << fixed << setprecision(1) 
               << matches[0].rating;
            bestMovieLines.push_back(ss.str());
        }
    }

    // Print best movies
    for (const auto& line : bestMovieLines) {
        cout << line << endl;
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

