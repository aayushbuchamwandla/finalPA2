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
#include <map>
using namespace std;

#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile(argv[1]);
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    string line, movieName;
    double movieRating;

    map<string, double> movies;
    vector<Movie> movies2;

    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
        movies[movieName] = movieRating;
        movies2.push_back(Movie(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
        for (auto it = movies.begin(); it != movies.end(); ++it) {
            cout << it->first << ", " << it->second << endl;
        }
        return 0;
    }

    ifstream prefixFile(argv[2]);
    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline(prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    vector<Movie> shortened;
    vector<string> finalLines;

    for (int i = 0; i < prefixes.size(); i++) {
        string prefixLower = prefixes[i];
        transform(prefixLower.begin(), prefixLower.end(), prefixLower.begin(), ::tolower);

        for (int j = 0; j < movies2.size(); j++) {
            string movieNameLower = movies2[j].name;
            transform(movieNameLower.begin(), movieNameLower.end(), movieNameLower.begin(), ::tolower);

            if (movieNameLower.find(prefixLower) == 0) {
                shortened.push_back(Movie(movies2[j].name, movies2[j].rating));
            }
        }

        PrintByRating(shortened);

        if (!shortened.empty()) {
            stringstream ss;
            ss << "Best movie with prefix " << prefixes[i] << " is: " << shortened[0].name
               << " with rating " << fixed << setprecision(1) << shortened[0].rating;
            finalLines.push_back(ss.str());
        } else {
            cout << "No movies found with prefix " << prefixes[i] << endl;
        }

        shortened.clear();
        cout << endl;
    }

    for (const string& s : finalLines) {
        cout << s << endl;
    }

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block */

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex + 1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
