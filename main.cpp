#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "movies.h"

using namespace std;

// Sort by movie name alphabetically
bool movieAlphabetical(const Movie& a, const Movie& b) {
    return a.name < b.name;
}

// Sort by rating (high to low), then name (alphabetically)
bool operations(const Movie& a, const Movie& b) {
    if (a.rating != b.rating)
        return a.rating > b.rating;
    return a.name < b.name;
}

// Binary search to find the first index where movie name starts with prefix
int lowerBoundPrefix(const vector<Movie>& movies, const string& prefix) {
    int low = 0, high = movies.size();
    while (low < high) {
        int mid = (low + high) / 2;
        if (movies[mid].name.compare(0, prefix.size(), prefix) < 0)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./runMovies movieFile [prefixFile]" << endl;
        return 1;
    }

    // Read movie data
    ifstream movieFile(argv[1]);
    if (!movieFile) {
        cerr << "Error opening movie file." << endl;
        return 1;
    }

    vector<Movie> movies;
    string line;
    while (getline(movieFile, line)) {
        size_t comma = line.rfind(',');
        if (comma == string::npos) continue;
        string name = line.substr(0, comma);
        double rating = stod(line.substr(comma + 1));
        movies.emplace_back(name, rating);
    }
    movieFile.close();

    // Sort all movies alphabetically
    sort(movies.begin(), movies.end(), movieAlphabetical);

    // If no prefix file is provided
    if (argc == 2) {
        for (const Movie& m : movies) {
            cout << m.name << ", " << m.rating << endl;
        }
        return 0;
    }

    // Read prefixes
    ifstream prefixFile(argv[2]);
    if (!prefixFile) {
        cerr << "Error opening prefix file." << endl;
        return 1;
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        prefixes.push_back(line);
    }
    prefixFile.close();

    // Process each prefix
    for (const string& prefix : prefixes) {
        vector<Movie> matches;
        int idx = lowerBoundPrefix(movies, prefix);
        while (idx < movies.size() &&
               movies[idx].name.compare(0, prefix.size(), prefix) == 0) {
            matches.push_back(movies[idx]);
            idx++;
        }

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            sort(matches.begin(), matches.end(), operations);
            for (const Movie& m : matches) {
                cout << m.name << ", " << m.rating << endl;
            }
            cout << "Best movie with prefix " << prefix
                 << " is: " << matches[0].name
                 << " with rating " << matches[0].rating << endl;
        }
    }

    return 0;
}
