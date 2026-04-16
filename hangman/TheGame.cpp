#include <iostream>   // biblioteca pentru input/output
#include <string>     // biblioteca pentru string
#include <cstdlib>    // biblioteca pentru system(), rand(), srand()
#include <ctime>      // biblioteca pentru time()
#include <windows.h>  // biblioteca Windows
#include <cctype>     // biblioteca pentru toupper()
#include <chrono>     // biblioteca pentru cronometru

using namespace std;  // evităm să scriem std:: peste tot

// Funcție care verifică dacă caracterul introdus este literă
bool isValidLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Funcție pentru curățarea bufferului de intrare
void clearInputBuffer() {
    cin.clear();              // resetează erorile fluxului
    cin.ignore(10000, '\n');  // ignoră caracterele rămase până la Enter
}

// Funcție pentru ștergerea ecranului consolei
void clearScreen() {
    system("cls"); // comandă Windows pentru curățarea ecranului
}

// Funcție pentru afișarea literelor greșite separate prin virgulă
string formatLetters(const string& letters) {
    if (letters.empty()) {       // dacă nu există litere
        return "niciuna";        // returnează "niciuna"
    }

    string result = "";          // șir pentru rezultat

    for (size_t i = 0; i < letters.length(); i++) {
        result += letters[i];    // adaugă litera curentă
        if (i < letters.length() - 1) {
            result += ", ";      // adaugă virgulă între litere
        }
    }

    return result;               // returnează șirul formatat
}

// Enumerare pentru nivelul de dificultate
enum Difficulty { EASY = 1, MEDIUM = 2, HARD = 3 };

// Funcție pentru alegerea dificultății
Difficulty selectDifficulty() {
    cout << "\n";
    cout << "========================================================\n";
    cout << "          SELECTEAZA NIVELUL DE DIFICULTATE\n";
    cout << "========================================================\n\n";
    cout << "1. USOR (fara timp limita)\n";
    cout << "2. MEDIU (60 secunde)\n";
    cout << "3. GREU (30 secunde)\n\n";

    int choice;          // variabilă pentru alegere
    bool valid = false;  // validare alegere

    while (!valid) {
        cout << "Alege (1-3): ";

        if (cin >> choice && choice >= 1 && choice <= 3) {
            clearInputBuffer(); // curăță buffer
            valid = true;
            return static_cast<Difficulty>(choice); // convertește la Difficulty
        } else {
            cout << "EROARE! Introduce 1, 2 sau 3!\n";
            clearInputBuffer();
        }
    }

    return EASY; // fallback
}

// Funcție care returnează lista de cuvinte după dificultate
string* getWordsForDifficulty(Difficulty diff, int& count) {

    // Lista de cuvinte ușoare
    static string easyWords[] = {
        "GANDA", "PISICA", "CAINE", "CASA", "MASINA"
    };

    // Lista de cuvinte medii
    static string mediumWords[] = {
        "PROGRAMARE", "CALCULATOR", "MONITOR"
    };

    // Lista de cuvinte grele
    static string hardWords[] = {
        "UNIVERSITATE", "PROGRAMARE", "ALGORITM"
    };

    if (diff == EASY) {
        count = sizeof(easyWords) / sizeof(easyWords[0]); // număr cuvinte
        return easyWords;
    }
    else if (diff == MEDIUM) {
        count = sizeof(mediumWords) / sizeof(mediumWords[0]);
        return mediumWords;
    }
    else {
        count = sizeof(hardWords) / sizeof(hardWords[0]);
        return hardWords;
    }
}

int main() {

    srand(time(0)); // inițializează generatorul aleator

    bool playAgain = true; // control repetare joc
    int roundNumber = 1;   // număr rundă

    // Mesaj de început
    cout << "BINE AI VENIT LA SPANZURATOARE!\n";

    while (playAgain) {

        Difficulty currentDifficulty = selectDifficulty(); // alege dificultatea

        int wordCount = 0; // număr de cuvinte

        string* wordsArray = getWordsForDifficulty(currentDifficulty, wordCount);

        int timeLimit = 0; // timp limită

        // stabilește timpul după dificultate
        if (currentDifficulty == EASY) timeLimit = 0;
        else if (currentDifficulty == MEDIUM) timeLimit = 60;
        else timeLimit = 30;

        // alege un cuvânt random
        string word = wordsArray[rand() % wordCount];

        // creează afișarea cu "_"
        string display(word.length(), '_');

        string guessed = "";         // litere greșite
        string guessedLetters = "";  // toate literele introduse

        int wrong = 0;       // număr greșeli
        int max_wrong = 6;   // maxim greșeli

        bool won = false;        // verifică dacă a câștigat
        bool timeExpired = false;// verifică dacă timpul a expirat

        auto startTime = chrono::high_resolution_clock::now(); // pornește cronometrul

        while (wrong < max_wrong && !won && !timeExpired) {

            clearScreen(); // șterge ecranul

            cout << "CUVANT: " << display << "\n";
            cout << "GRESITE: " << formatLetters(guessed) << "\n";

            // verifică timpul
            if (timeLimit > 0) {
                auto currentTime = chrono::high_resolution_clock::now();
                auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

                if (elapsedSeconds >= timeLimit) {
                    timeExpired = true;
                    break;
                }
            }

            cout << "INTRODU O LITERA: ";

            char letter;
            cin >> letter; // citește litera

            letter = toupper(letter); // transformă în majusculă

            // dacă litera a fost deja introdusă
            if (guessedLetters.find(letter) != string::npos) {
                cout << "Litera deja introdusa!\n";
                continue;
            }

            guessedLetters += letter; // adaugă litera în lista totală

            bool found = false;

            // caută litera în cuvânt
            for (size_t i = 0; i < word.length(); i++) {
                if (word[i] == letter) {
                    display[i] = letter; // descoperă litera
                    found = true;
                }
            }

            // dacă nu a găsit litera
            if (!found) {
                wrong++;           // crește numărul de greșeli
                guessed += letter; // adaugă la litere greșite
            }

            // verifică dacă tot cuvântul a fost ghicit
            if (display == word) {
                won = true;
            }
        }

        // rezultat final
        if (won) {
            cout << "FELICITARI! AI CASTIGAT!\n";
        }
        else if (timeExpired) {
            cout << "TIMP EXPIRAT!\n";
        }
        else {
            cout << "AI PIERDUT! CUVANTUL ERA: " << word << "\n";
        }

        // întreabă dacă vrea să joace din nou
        cout << "VREI SA JOCI DIN NOU? (D/N): ";
        char response;
        cin >> response;

        response = toupper(response);

        if (response == 'D') {
            playAgain = true;
            roundNumber++;
        }
        else {
            playAgain = false;
            cout << "MULTUMESC CA AI JUCAT!\n";
        }
    }

    return 0; // termină programul
}