#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include "MovieManager.h"
#include "utils.h"
using namespace std;

//the compiling command and runiing command for all the files
//g++ main.cpp utils.cpp MovieNode.cpp ActorNode.cpp ActorHashTable.cpp MovieGraph.cpp MovieManager.cpp -o program.exe
//.\program.exe

// ANSI Color Codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_CYAN "\033[46m"


int main() {

    ifstream file("movie_metadata.csv");
    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        return 1;
    }

    MovieManager manager;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        //for 28 columns
        char col[28][500];

        //initializing columns
        for (int i = 0; i < 28; i++)
            col[i][0] = '\0';

        string temp;
        //index represnets which col we are filling 
        int index = 0;
        while (getline(ss, temp, ',')) {
            if (index < 28)
                //Copies the extracted value safely into col[index]
                //strcpy ni chl rha tha in vs22
                snprintf(col[index], 500, "%s", temp.c_str());
            index++;
        }
        for (int i = index; i < 28; i++)
            col[i][0] = '\0';

        char* director_name = col[1];
        char* duration = col[3];
        char* actor_2_name = col[6];
        char* genres = col[9];
        char* actor_1_name = col[10];
        char* movie_title = col[11];
        char* actor_3_name = col[14];
        char* budget = col[22];
        char* title_year = col[23];
        char* imdb_score = col[25];

        clean(director_name);
        clean(duration);
        clean(actor_2_name);
        clean(genres);
        clean(actor_1_name);
        clean(movie_title);
        clean(actor_3_name);
        clean(budget);
        clean(title_year);
        clean(imdb_score);

        string d_director = fixMissing(director_name, "Unknown");
        string d_duration = fixMissing(duration, "0");
        string d_actor2 = fixMissing(actor_2_name, "Unknown");
        string d_genres = fixMissing(genres, "Unknown");
        string d_actor1 = fixMissing(actor_1_name, "Unknown");
        string d_title = fixMissing(movie_title, "Unknown");
        string d_actor3 = fixMissing(actor_3_name, "Unknown");
        string d_budget = fixMissing(budget, "0");
        string d_year = fixMissing(title_year, "0");
        string d_score = fixMissing(imdb_score, "0");

        // each movie is stored as node with the extracted fileds 
        MovieNode* movie = new MovieNode(
            d_director, d_duration, d_actor2, d_genres,
            d_actor1, d_title, d_actor3, d_budget,
            d_year, d_score
        );

        movie->populateGenres(d_genres);

        manager.addMovie(movie);
    }

    int choice;
    string input;
    float low, high;

    while (true) {
        cout << CYAN << BOLD;

        // Top border
        cout << "+----------------------------------------------------------+\n";

        // Title
        cout << "|" << BG_CYAN << BOLD << "                 MOVIE DATABASE SYSTEM                    " << RESET << CYAN << "|\n";

        // Separator after title
        cout << "+----------------------------------------------------------+\n";

        // SEARCH OPERATIONS section
        cout << "| " << YELLOW << BOLD << "SEARCH OPERATIONS" << RESET << CYAN << string(40, ' ') << "|\n";
        cout << "+----------------------------------------------------------+\n";

        cout << "|" << "  " << GREEN << "1." << RESET << CYAN << " Search by Title" << string(38, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "2." << RESET << CYAN << " Search by Actor (list movies)" << string(24, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "3." << RESET << CYAN << " Print Actor Filmography" << string(30, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "4." << RESET << CYAN << " Print Actor Co-Actors" << string(32, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "5." << RESET << CYAN << " Search by Genre" << string(38, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "6." << RESET << CYAN << " Search by Year" << string(39, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "7." << RESET << CYAN << " Search by Rating Range" << string(31, ' ') << "|\n";

        // Separator
        cout << "+----------------------------------------------------------+\n";

        // DATABASE OPERATIONS section
        cout << "|" << "  " << YELLOW << BOLD << "DATABASE OPERATIONS" << RESET << CYAN << string(37, ' ') << "|\n";
        cout << "+----------------------------------------------------------+\n";

        cout << "|" << "  " << GREEN << "8." << RESET << CYAN << " Delete Movie" << string(41, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "9." << RESET << CYAN << " Display All Movies" << string(35, ' ') << "|\n";

        // Separator
        cout << "+----------------------------------------------------------+\n";

        // GRAPH OPERATIONS section
        cout << "|" << "  " << YELLOW << BOLD << "GRAPH OPERATIONS" << RESET << CYAN << string(40, ' ') << "|\n";
        cout << "+----------------------------------------------------------+\n";

        cout << "|" << "  " << GREEN << "10." << RESET << CYAN << " Build Movie Graph" << string(35, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "11." << RESET << CYAN << " Show Movie Connections" << string(30, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "12." << RESET << CYAN << " Display Graph Statistics" << string(28, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "13." << RESET << CYAN << " Get Movie Recommendations" << string(27, ' ') << "|\n";
        cout << "|" << "  " << GREEN << "14." << RESET << CYAN << " Find Shortest Path Between Movies" << string(19, ' ') << "|\n";

        // Separator
        cout << "+----------------------------------------------------------+\n";

        // Exit option
        cout << "|" << "  " << RED << BOLD << "0." << RESET << CYAN << " Exit" << string(49, ' ') << "|\n";

        // Bottom border
        cout << "+----------------------------------------------------------+\n";
        cout << RESET;

        // Input prompt
        cout << "\n" << MAGENTA << BOLD << ">>> " << RESET << CYAN << "Enter your choice" << MAGENTA << ": " << RESET;

        cin >> choice;
        cin.ignore();
        cout << "\n";

        switch (choice) {
        case 1:
            cout << "Enter title: ";
            getline(cin, input);
            manager.searchByTitle(input);
            break;

        case 2:
            cout << "Enter actor name: ";
            getline(cin, input);
          
            manager.searchByActor(input);
            break;

        case 3:
            cout << "Enter actor name: ";
            getline(cin, input);
            manager.printActorFilmography(input);
            break;

        case 4:
            cout << "Enter actor name: ";
            getline(cin, input);
            manager.printActorCoActors(input);
            break;

        case 5:
            cout << "Enter genre: ";
            getline(cin, input);
            manager.searchByGenre(input);
            break;

        case 6:
            cout << "Enter year: ";
            getline(cin, input);
            manager.searchByYear(input);
            break;

        case 7:
            cout << "Enter min rating: ";
            cin >> low;
            cout << "Enter max rating: ";
            cin >> high;
            cin.ignore();
            manager.searchByRating(low, high);
            break;

        case 8:
            cout << "Enter title to delete: ";
            getline(cin, input);
            manager.deleteMovie(input);
            break;

        case 9:
            manager.display();
            break;
        case 10:
            manager.buildGraph();
            break;

        case 11:
            cout << "Enter movie title: ";
            getline(cin, input);
            manager.printMovieConnections(input);
            break;

        case 12:
            manager.displayGraphStats();
            break;
        case 13: {
            cout << "Enter movie title: ";
            getline(cin, input);
            cout << "How many recommendations (default 10)? ";
            string numStr;
            getline(cin, numStr);
            int numRecs = numStr.empty() ? 10 : stoi(numStr);
            manager.recommendMovies(input, numRecs);
            break;
        }
        case 14: {
            string movie1, movie2;
            cout << "Enter first movie title: ";
            getline(cin, movie1);
            cout << "Enter second movie title: ";
            getline(cin, movie2);
            manager.findShortestPath(movie1, movie2);
            break;
        }

        case 0: {
            cout << "\n" << CYAN << BOLD;
            cout << "+----------------------------------------------------------+\n";
            cout << "|" << "  " << BG_CYAN << "           Thank you for using our system!           " << RESET << CYAN << "  " << "|\n";
            cout << "|" << "  " << GREEN << "                    Goodbye!                    " << RESET << CYAN << "  " << "|\n";
            cout << "+----------------------------------------------------------+\n";
            cout << RESET << "\n";
            return 0;
        }

        default:
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}

