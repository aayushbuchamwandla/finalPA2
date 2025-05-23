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
    // Create an object of a STL data-structure to store all the movies

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating

    map<string,double> movies;
    vector<Movie> movies2;
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Algorithm 1 (movies[movieName] = movieRating;)
            movies[movieName] = movieRating;
            //Algorithm 2
            movies2.push_back(Movie(movieName, movieRating));
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
    }

    movieFile.close();

    if (argc == 2){
        //Algorithm 1
        //print all the movies in ascending alphabetical order of movie names
        for (auto it = movies.begin(); it != movies.end(); ++it) {
            cout<<it->first<<", "<<it->second<<endl;
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

    vector<Movie> shortened;
    vector<string> finalLines;
    for (int i=0; i<prefixes.size(); i++) {
        for (int j=0; j<movies2.size(); j++) {
            if ((movies2.at(j).name).find(prefixes.at(i))==0) {
                shortened.push_back(Movie(movies2.at(j).name,movies2.at(j).rating));
            }
        }
        PrintByRating(shortened);
        if (shortened.size()!=0) {
            stringstream ss;
            ss.str("");
            ss.clear();
            ss<<"Best movie with prefix "<<prefixes.at(i)<<" is: "<<shortened.at(0).name<<" with rating "<<std::fixed<<std::setprecision(1)<<shortened.at(0).rating;
            line = ss.str();
            finalLines.push_back(line);
        }
        else {
            cout<<"No movies found with prefix " + prefixes.at(i);
        }
        shortened.clear();
        cout<<endl;
    }
    for (string s: finalLines) {
        cout<<s<<endl;
    }
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
