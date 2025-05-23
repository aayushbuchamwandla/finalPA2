#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Movie {
    string name;
    int rating;
};

// Compare movies alphabetically by name
bool compareByName(const Movie& a, const Movie& b) {
    return a.name < b.name;
}

// Compare movies by rating (desc), then name (asc)
bool compareByRatingThenName(const Movie& a, const Movie& b) {
    if (a.rating != b.rating) return a.rating > b.rating;
    return a.name < b.name;
}

// Binary search lower bound for prefix
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

int main() {
    int n;
    cin >> n;
    vector<Movie> movies(n);

    for (int i = 0; i < n; ++i) {
        cin >> ws; // ignore any whitespace
        getline(cin, movies[i].name);
        cin >> movies[i].rating;
    }

    sort(movies.begin(), movies.end(), compareByName); // sort once by name

    int prefixCount;
    cin >> prefixCount;
    vector<string> prefixes(prefixCount);
    for (int i = 0; i < prefixCount; ++i) {
        cin >> ws;
        getline(cin, prefixes[i]);
    }

    for (const string& prefix : prefixes) {
        vector<Movie> matches;
        int index = lowerBoundPrefix(movies, prefix);

        // Collect all matching prefix movies
        while (index < movies.size() &&
               movies[index].name.compare(0, prefix.size(), prefix) == 0) {
            matches.push_back(movies[index]);
            index++;
        }

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            sort(matches.begin(), matches.end(), compareByRatingThenName);
            cout << "Best movie with prefix " << prefix
                 << " is: " << matches[0].name
                 << " with rating " << matches[0].rating << endl;
        }
    }

    return 0;
}
