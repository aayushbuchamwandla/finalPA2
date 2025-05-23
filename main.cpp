#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "movies.h"

using namespace std;

// Comparator for sorting movies alphabetically by name
bool movieAlphabetical(const Movie& m1, const Movie& m2) {
    return m1.name < m2.name;
}

// Binary search to find the first index of a movie that starts with the prefix
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
        cerr << "Usage: ./runMovies movieFilename [prefixFilename]" << endl;
        return 1;
    }

    string movieFilename = argv[1];
    ifstream movieFile(movieFilename);
    if (!movieFile) {
        cerr << "Error opening movie file." << endl;
        return 1;
    }

    vector<Movie> movies;
    string line;
    while (getline(movieFile, line)) {
        size_t commaPos = line.rfind(',');
        if (commaPos != string::npos) {
            string name = line.substr(0, commaPos);
            double rating = stod(line.substr(commaPos + 1));
            movies.emplace_back(name, rating);
        }
    }

    sort(movies.begin(), movies.end(), movieAlphabetical);

    if (argc == 2) {
        for (const auto& m : movies) {
            cout << m.name << ", " << m.rating << endl;
        }
    } else {
        string prefixFilename = argv[2];
        ifstream prefixFile(prefixFilename);
        if (!prefixFile) {
            cerr << "Error opening prefix file." << endl;
            return 1;
        }

        vector<string> prefixes;
        while (getline(prefixFile, line)) {
            prefixes.push_back(line);
        }

        vector<string> bestResults;

        for (const string& prefix : prefixes) {
            vector<Movie> matched;
            int idx = lowerBoundPrefix(movies, prefix);
            while (idx < movies.size() && movies[idx].name.compare(0, prefix.size(), prefix) == 0) {
                matched.push_back(movies[idx]);
                ++idx;
            }

            if (matched.empty()) {
                cout << "No movies found with prefix " << prefix << endl;
                bestResults.push_back("");  // Placeholder
            } else {
                sort(matched.begin(), matched.end(), operations);
                for (const auto& m : matched) {
                    cout << m.name << ", " << m.rating << endl;
                }
                bestResults.push_back("Best movie with prefix " + prefix + " is: " + matched[0].name + " with rating " + to_string(matched[0].rating));
            }
        }

        for (const auto& res : bestResults) {
            if (!res.empty())
                cout << res << endl;
        }
    }

    return 0;
}
