#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "movies.h"

bool parseLine(std::string &line, std::string &movieName, double &movieRating);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Not enough arguments provided (need at least 1 argument)." << std::endl;
        std::cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << std::endl;
        return 1;
    }

    std::ifstream movieFile(argv[1]);
    if (movieFile.fail()) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<Movie> movies;
    std::string line, movieName;
    double movieRating;

    // Read and store all movies
    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
        movies.emplace_back(movieName, movieRating);
    }
    movieFile.close();

    // Sort movies by name once
    std::sort(movies.begin(), movies.end());

    if (argc == 2) {
        // Print all movies in alphabetical order
        for (const auto& movie : movies) {
            std::cout << movie.name << ", " << movie.rating << std::endl;
        }
        return 0;
    }

    std::ifstream prefixFile(argv[2]);
    if (prefixFile.fail()) {
        std::cerr << "Could not open file " << argv[2] << std::endl;
        return 1;
    }

    std::vector<std::string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();

    std::vector<Movie> matches;
    std::vector<std::string> bestMovieLines;

    for (const auto& prefix : prefixes) {
        findMoviesByPrefix(movies, prefix, matches);

        if (matches.empty()) {
            std::cout << "No movies found with prefix " << prefix << std::endl;
        } else {
            // Print all matches
            for (const auto& movie : matches) {
                std::cout << movie.name << ", " << movie.rating << std::endl;
            }
            std::cout << std::endl;

            // Store best movie line
            std::stringstream ss;
            ss << "Best movie with prefix " << prefix << " is: " 
               << matches[0].name << " with rating " << std::fixed << std::setprecision(1) 
               << matches[0].rating;
            bestMovieLines.push_back(ss.str());
        }
    }

    // Print best movies
    for (const auto& line : bestMovieLines) {
        std::cout << line << std::endl;
    }

    return 0;
}

bool parseLine(std::string &line, std::string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}

