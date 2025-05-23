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

#include "movies.h"

using namespace std;

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    if (commaIndex == string::npos) {
        return false;
    }
    movieName = line.substr(0, commaIndex);
    try {
        movieRating = stod(line.substr(commaIndex + 1));
    } catch (const invalid_argument& e) {
        cerr << "Invalid rating format: " << line.substr(commaIndex + 1) << endl;
        return false;
    } catch (const out_of_range& e) {
        cerr << "Rating out of range: " << line.substr(commaIndex + 1) << endl;
        return false;
    }

    if (movieName.length() >= 2 && movieName[0] == '\"' && movieName.back() == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}

int main(int argc, char** argv){
    clock_t start_time = clock();

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

    vector<Movie> allMovies;

    string line;
    string movieName;
    double movieRating;

    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        allMovies.push_back(Movie(movieName, movieRating));
    }
    movieFile.close();

    std::sort(allMovies.begin(), allMovies.end());

    if (argc == 2){
        for (const auto& movie : allMovies) {
            cout << movie.name << ", " << movie.rating << endl;
        }
        clock_t end_time = clock();
        double elapsed_secs = double(end_time - start_time) / CLOCKS_PER_SEC;
        cerr << "Total execution time: " << elapsed_secs * 1000 << " ms" << endl;
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
    prefixFile.close();

    vector<string> bestMovieOutputLines;

    for (const string& prefix : prefixes) {
        auto lower_it = std::lower_bound(allMovies.begin(), allMovies.end(), Movie(prefix, 0.0),
            [](const Movie& m, const Movie& p) {
                return m.name < p.name;
            });

        string upper_bound_str = prefix;
        if (!upper_bound_str.empty()) {
            size_t last_char_idx = upper_bound_str.length() - 1;
            char last_char = upper_bound_str[last_char_idx];
            if (last_char == '~' || last_char == 255) {
                upper_bound_str += '~';
            } else {
                upper_bound_str[last_char_idx]++;
            }
        } else {
            upper_bound_str = "~";
        }

        auto upper_it = std::upper_bound(allMovies.begin(), allMovies.end(), Movie(upper_bound_str, 0.0),
            [](const Movie& m, const Movie& p) {
                return m.name < p.name;
            });

        vector<Movie> matchingMovies;
        for (auto it = lower_it; it != upper_it; ++it) {
            if (it->name.rfind(prefix, 0) == 0) {
                matchingMovies.push_back(*it);
            }
        }

        if (matchingMovies.empty()) {
            cout << "No movies found with prefix " << prefix;
        } else {
            PrintByRating(matchingMovies);
            cout << endl;
        }

        if (!matchingMovies.empty()) {
            stringstream ss;
            ss.str("");
            ss.clear();
            ss << "Best movie with prefix " << prefix << " is: "
               << matchingMovies.at(0).name << " with rating "
               << std::fixed << std::setprecision(1) << matchingMovies.at(0).rating;
            bestMovieOutputLines.push_back(ss.str());
        }
    }

    for (const string& s : bestMovieOutputLines) {
        cout << s << endl;
    }

    clock_t end_time = clock();
    double elapsed_secs = double(end_time - start_time) / CLOCKS_PER_SEC;
    cerr << "Total execution time: " << elapsed_secs * 1000 << " ms" << endl;

    return 0;
}
