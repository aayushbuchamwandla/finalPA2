// Spring '25
// Instructor: Diba Mirza
// Student name: Ashwin Kannan, Aayush Buchamwandla
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

#include "utilities.h"
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

    string line, movieName;
    double movieRating;
    vector<Movie> movies;

    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        Movie m(movieName, movieRating);
        movies.push_back(m);
    }
    
    movieFile.close();

    sort(movies.begin(), movies.end());

    if (argc == 2) {
        for (const auto& movie : movies) {
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

    vector<Movie> matches;
    vector<string> bestMovieLines;

    for (const auto& prefix : prefixes) {
        findMoviesByPrefix(movies, prefix, matches);

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            // Print all matches
            for (const auto& movie : matches) {
                cout << movie.name << ", " << movie.rating << endl;
            }
            cout << endl;

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

/* Add your run time analysis for part 3 of the assignment here as commented block*/
/*
Pa02 Part 3 - Analysis
    
3a -
The algorithm consists of several steps -
    ● Sorting the movies o(nlogn) using sort
    ● Processing each prefix (m prefixes total)
        ○ Finding movies with the prefix using lower_bound 0(logn) per prefix
        ○ Creating a sublist of matches o(k) per prefix (copying elements)
        ○ Sorting matches by rating 0(klogk) per prefix
Key Parameters -
    ● N - number of movies
    ● M - number of prefixes
    ● K - maximum number of movies matching any prefix
Total Time Complexity -
    ● Preprocessing (sorting) - o(nlogn)
    ● For each prefix
        ○ Binary search - o(log n)
        ○ Copy matches - o(k)
        ○ Sort matches - o(klogk)
    ● Total for all prefixes: o(m*(log n + k +klogk))
Worst case occurs when k ~ n (all movies match a prefix), giving o(m*(logn+n+nlogn)) (mnlogn) However, with typical inputs where k << n, the dominant term is o(nlogn+mlogn) from the initial sort and binary searches
Empirical runtime observations
    1. Input_20_random.csv - 0.0529s
    2. Input_100_random.csv - 0.0644s
    3. Input_1000_random.csv - 0.1146s
    4. Input_76920_random.csv ~ 1.5s (estimated)
    
The runtime grows slightly worse than linear with n, which matches out analysis since the o(nlogn) sort dominates for larger n


Part 3b - Space Complexity Analysis

The algorithm uses several data structures
    1. Vector to store all movies - o(n)
    2. Vector to store all prefixes - o(m*L) where L is average prefix length
    3. Temporary vector for matches: o(k) during processing each prefix

Additional space for sorting -
    ● Sort typically uses o(log n) space for recursion
    ● Sort of matches uses 0(log k) space

Total Space Complexity -
    ● O(n+m*L+k) - since k is temporary and can be reused
In worst case where k ~ n (all movies match a prefix), space becomes 0(n+m*L)
The space usage is dominated by the storage of all movies O(n) and is efficient relative to input size


3c -
Design choices focusing on time complexity
    1. Pre-sorting movies alphabetically o(nlogn) enables efficient o(log n) prefix searches
    2. Tradeoff - Uses O(n) space to store sorted movies
Optimizations made -
    ● Initially had linear search with o(n) run time - switched to binary search for prefix matching
    ● Only sort matching siubset for each prefix rather than all movies
    ● Reuse results vector to minimize allocations Achieved both low time and space complexity
    ● Time - o(nlogn+mlogn) in typical case
    ● Space - o(n)which optimal for storing input Difficulties:
    ● Achieving low time complexity was more challenging, particularly -
        ○ Handling the multiple sorting requirements (alphabetical for search, then by rating
        for output)
        ○ Efficiengtly finding all movies with a given prefix without scanning entire list
The solution successfully implements binary search and balances both time and space efficiency by -
    ● Sorting all movies upfront o(nlogn) which allows much faster later search
    ● Leveraging sorted order for efficient searches
    ● Minimizing temporary storage
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
