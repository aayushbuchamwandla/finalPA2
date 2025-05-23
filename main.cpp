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

#include "movies.h"  // Include our Movie struct and PrintByRating function

using namespace std;

// Function to parse a line from the CSV file
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

    std::sort(movies2.begin(), movies2.end());

    if (argc == 2){
        for (const auto& movie : movies2) {
            cout << movie.name << ", " << movie.rating << endl;
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
    prefixFile.close();

    vector<string> finalLines;

    for (int i=0; i<prefixes.size(); i++) {
        const string& prefix = prefixes.at(i);

        auto lower_it = std::lower_bound(movies2.begin(), movies2.end(), Movie(prefix, 0.0),
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

        auto upper_it = std::upper_bound(movies2.begin(), movies2.end(), Movie(upper_bound_str, 0.0),
            [](const Movie& m, const Movie& p) {
                return m.name < p.name;
            });

        vector<Movie> shortened;
        for (auto it = lower_it; it != upper_it; ++it) {
            if (it->name.find(prefix) == 0) {
                shortened.push_back(*it);
            }
        }

        PrintByRating(shortened);

        if (shortened.size()!=0) {
            stringstream ss;
            ss.str("");
            ss.clear();
            ss<<"Best movie with prefix "<<prefix<<" is: "<<shortened.at(0).name<<" with rating "<<std::fixed<<std::setprecision(1)<<shortened.at(0).rating;
            finalLines.push_back(ss.str());
            cout<<endl;
        } else {
            cout<<"No movies found with prefix " + prefix;
        }
    }

    for (string s : finalLines) {
        cout << s << endl;
    }

    return 0;
}
