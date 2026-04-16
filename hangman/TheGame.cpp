#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <cctype>
#include <chrono>
using namespace std;

// Functie pentru validare input litera
bool isValidLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Functie pentru curatare input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// Functie pentru stergerea ecranului
void clearScreen() {
    system("cls");
}

// Functie pentru formatare litere cu separatori
string formatLetters(const string& letters) {
    if (letters.empty()) {
        return "niciuna";
    }
    string result = "";
    for (size_t i = 0; i < letters.length(); i++) {
        result += letters[i];
        if (i < letters.length() - 1) {
            result += ", ";  // Adauga virgula si spatiu dupa fiecare litera
        }
    }
    return result;
}

// Enum pentru dificultate
enum Difficulty { EASY = 1, MEDIUM = 2, HARD = 3 };

// Functie pentru selectarea dificultații
Difficulty selectDifficulty() {
    cout << "\n";
    cout << "========================================================\n";
    cout << "          SELECTEAZA NIVELUL DE DIFICULTATE\n";
    cout << "========================================================\n\n";
    cout << "1. USOR (fara timp limita)\n";
    cout << "2. MEDIU (60 secunde)\n";
    cout << "3. GREU (30 secunde)\n\n";
    
    int choice;
    bool valid = false;
    
    while (!valid) {
        cout << "Alege (1-3): ";
        if (cin >> choice && choice >= 1 && choice <= 3) {
            clearInputBuffer();
            valid = true;
            return static_cast<Difficulty>(choice);
        } else {
            cout << "EROARE! Introduce 1, 2 sau 3!\n";
            clearInputBuffer();
        }
    }
    return EASY;
}

// Functie pentru obținerea cuvintelor în funcție de dificultate
string* getWordsForDifficulty(Difficulty diff, int& count) {
    // Cuvinte USOR (simple, 4-5 litere)
    static string easyWords[] = {"GANDA", "PISICA", "CAINE", "CASA", "MASINA", 
                                  "TELEFON", "LAPTOP", "MOUSE", "SCAUN", "MASA",
                                  "USA", "FLOARE", "COPAC", "LUNA", "STEA",
                                  "MAMA", "TATA", "FRATE", "SORA", "PAN",
                                  "PAINE", "LAPTE", "ORAS", "DRUM", "BANCA"};
    
    // Cuvinte MEDIU (5-8 litere)
    static string mediumWords[] = {"PROGRAMARE", "CALCULATOR", "MONITOR", "TASTATURA", 
                                    "BIROU", "FEREASTRA", "PERETE", "PASARE", "PESTE", 
                                    "MELCUL", "SOARE", "NORI", "VANT", "PLOAIE",
                                    "BIBLIOTECA", "SCOALA", "PROFESOR", "STUDENT",
                                    "MANCARE", "CARNE", "LEGUME", "FRUCT", "CIOCOLATA",
                                    "MUNCA", "JUCARIE", "CARTEA", "REVISTA"};
    
    // Cuvinte GREU (8+ litere, mai greu de ghicit)
    static string hardWords[] = {"UNIVERSITATE", "CALCULATOR", "PROGRAMARE", "COMUTER",
                                  "ADMINISTRARE", "MATEMATICA", "FIZICA", "BIOLOGIE",
                                  "PSIHOLOGIE", "SOCIOLOGIE", "GEOGRAFIE", "ASTRONOMIE",
                                  "CIBERNETIC", "MICROPROCESSOR", "ARQUITECTURA", "DEZVOLTARE",
                                  "IMPLEMENTARE", "ALGORITM", "STRUCTURA", "MEMORIE"};
    
    if (diff == EASY) {
        count = sizeof(easyWords) / sizeof(easyWords[0]);
        return easyWords;
    } else if (diff == MEDIUM) {
        count = sizeof(mediumWords) / sizeof(mediumWords[0]);
        return mediumWords;
    } else {
        count = sizeof(hardWords) / sizeof(hardWords[0]);
        return hardWords;
    }
}

int main() {
    srand(time(0));
    
    bool playAgain = true;
    int roundNumber = 1;
    
    // INCEPUT
    cout << "\n";
    cout << "========================================================\n";
    cout << "              BINE AI VENIT LA SPANZURATOARE!\n";
    cout << "========================================================\n\n";
    
    cout << "CUM SE JOACA:\n";
    cout << "1. Trebuie sa ghicesti un cuvant din limba romana\n";
    cout << "2. Ai maxim 6 greseli (apoi se termina jocul)\n";
    cout << "3. Vezi desenul spanzuratoarei in stanga\n";
    cout << "4. Cu fiecare gresala se adauga o parte a omului\n";
    cout << "5. Ghiceste cuvantul inainte sa se termine desenul!\n";
    cout << "6. Respecta TIMP LIMITA pentru modul MEDIU si GREU!\n\n";
    
    cout << "========================================================\n";
    
    while (playAgain) {
        // Selecteaza dificultatea
        Difficulty currentDifficulty = selectDifficulty();
        int wordCount = 0;
        string* wordsArray = getWordsForDifficulty(currentDifficulty, wordCount);
        
        // Determina timp limita
        int timeLimit = 0; // 0 = fara timp
        string difficultyName = "";
        
        if (currentDifficulty == EASY) {
            timeLimit = 0; // Fara timp
            difficultyName = "USOR";
        } else if (currentDifficulty == MEDIUM) {
            timeLimit = 60; // 60 secunde
            difficultyName = "MEDIU";
        } else {
            timeLimit = 30; // 30 secunde
            difficultyName = "GREU";
        }
        
        cout << "\nRUNDA " << roundNumber << " - NIVELUL: " << difficultyName;
        if (timeLimit > 0) {
            cout << " (TIMP: " << timeLimit << " secunde)";
        }
        cout << "\n";
        cout << "========================================================\n\n";
        
        // Selecteaza un cuvant aleatoriu
        string word = wordsArray[rand() % wordCount];
        
        // Variabile joc
        string display = string(word.length(), '_');
        string guessed = "";
        string guessedLetters = "";
        int wrong = 0;
        int max_wrong = 6;
        bool won = false;
        bool timeExpired = false;
        string lastMessage = "";  // Mesaj pentru feedback
        
        // Incepe cronometrul
        auto startTime = chrono::high_resolution_clock::now();
    
        // JOCUL
        while (wrong < max_wrong && !won && !timeExpired) {
            // Sterge ecranul pentru a actualiza afisarea pe aceeasi pozitie
            clearScreen();
            
            cout << "\nRUNDA " << roundNumber << " - NIVELUL: " << difficultyName;
            if (timeLimit > 0) {
                cout << " (TIMP: " << timeLimit << " secunde)";
            }
            cout << "\n";
            cout << "========================================================\n\n";
            
            // Verifica timp limita
            if (timeLimit > 0) {
                auto currentTime = chrono::high_resolution_clock::now();
                auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
                
                if (elapsedSeconds >= timeLimit) {
                    timeExpired = true;
                    break;
                }
            }
            
            cout << "\n";
            cout << "SPANZURATOARE:         STATUS:                     CUVANT:\n";
            cout << "===============        =======                     =======\n";
            
            // Linia 1
            cout << "  ------              Greseli: " << wrong << "/" << max_wrong;
            cout << "               " << display << "\n";
            
            // Linia 2 - Timp ramas (doar daca exista timp limita)
            cout << "  |    |";
            cout << "              Vieti ramase: " << (max_wrong - wrong);
            
            if (timeLimit > 0) {
                auto currentTime = chrono::high_resolution_clock::now();
                auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
                int remainingTime = timeLimit - elapsedSeconds;
                cout << "            Timp ramas: " << remainingTime << "s";
            }
            cout << "\n";
            
            // Linia 3 - Cap
            cout << "  |";
            if (wrong >= 1) cout << "    O";
            else cout << "     ";
            cout << "               Greseli: " << formatLetters(guessed) << "\n";
            
            // Linia 4 - Corp si brate
            cout << "  |";
            if (wrong >= 3) cout << "   /";
            else cout << "    ";
            if (wrong >= 2) cout << "|";
            else cout << " ";
            if (wrong >= 4) cout << "\\";
            else cout << " ";
            cout << "\n";
            
            // Linia 5 - Picioare
            cout << "  |";
            if (wrong >= 5) cout << "   /";
            else cout << "    ";
            if (wrong >= 6) cout << " \\";
            else cout << "  ";
            cout << "\n";
            
            // Linia 6 - Baza
            cout << " ---\n\n";
            
            // AFISEAZA MESAJUL DE FEEDBACK (daca exista)
            if (!lastMessage.empty()) {
                cout << lastMessage << "\n\n";
            }
            
            // INTRARE UTILIZATOR - VALIDATA SI SIGURA
            cout << "INTRODU O LITERA (A-Z): ";
            char letter;
            
            // Incearca pana cand input este valid
            bool validInput = false;
            while (!validInput) {
                if (cin >> letter) {
                    clearInputBuffer(); // Curata buffer-ul
                    
                    // Verifica daca e litera valida
                    if (isValidLetter(letter)) {
                        // Converteste la majuscula
                        if (letter >= 'a' && letter <= 'z') {
                            letter = letter - ('a' - 'A');
                        }
                        validInput = true;
                    } else {
                        cout << "EROARE! Introduce o litera VALIDA (A-Z): ";
                    }
                } else {
                    cout << "EROARE! Input invalid. Introduce o litera: ";
                    clearInputBuffer();
                }
            }
            
            cout << "\n";
            
            // VERIFICARE LITERA DUPLICATA
            if (guessedLetters.find(letter) != string::npos) {
                lastMessage = "ATENTIE! Ai ghicit deja litera [" + string(1, letter) + "]! Incearca alta!";
                continue;
            }
            
            guessedLetters += letter;
            
            // CAUTARE LITERA
            bool found = false;
            for (size_t i = 0; i < word.length(); i++) {
                if (word[i] == letter) {
                    display[i] = letter;
                    found = true;
                }
            }
            
            // REZULTAT
            if (!found) {
                wrong++;
                guessed += letter;
                lastMessage = "GRESIT! Litera [" + string(1, letter) + "] NU e in cuvant! Ai pierdut 1 viata!";
            } else {
                lastMessage = "CORECT! Litera [" + string(1, letter) + "] e in cuvant! Bravo!";
            }
            
            // VERIFICARE CASTIG
            if (display == word) {
                won = true;
            }
        }
        
        // REZULTAT FINAL
        cout << "\n";
        cout << "========================================================\n";
        cout << "              JOCUL S-A TERMINAT!\n";
        cout << "========================================================\n\n";
        
        // Desenul final
        cout << "\nDESENUL FINAL:\n";
        cout << "  ------\n";
        cout << "  |    |\n";
        if (wrong >= 1) cout << "  |    O\n";
        else cout << "  |\n";
        
        if (wrong >= 3) {
            if (wrong >= 4) {
                cout << "  |   /|\\\n";
            }
            else {
                cout << "  |   /|\n";
            }
        } else if (wrong >= 2) {
            cout << "  |    |\n";
        } else {
            cout << "  |\n";
        }
        
        if (wrong >= 5) {
            if (wrong >= 6) {
                cout << "  |   / \\\n";
            }
            else {
                cout << "  |   /\n";
            }
        } else {
            cout << "  |\n";
        }
        cout << " ---\n\n";
        
        // REZULTAT
        cout << "CUVANTUL ERA: " << word << "\n\n";
        cout << "STATISTICI:\n";
        cout << "  - Greseli: " << wrong << "/" << max_wrong << "\n";
        cout << "  - Litere gresit ghicite: " << (guessed.empty() ? "(niciuna)" : guessed) << "\n";
        
        if (timeExpired) {
            cout << "  - STATUS: TIMP EXPIRAT!\n\n";
            cout << "     *** JOCU S-A TERMINAT! TI S-A TERMINAT TIMPUL! ***\n";
            cout << "     Nu ai reusit sa ghicesti cuvantul la timp!\n";
        } else if (won) {
            cout << "\n";
            cout << "     *** FELICITARI! AI CASTIGAT! ***\n";
            cout << "     Ai salvat omul! Esti campion!\n";
        } else {
            cout << "\n";
            cout << "     *** JOCU S-A TERMINAT! AI PIERDUT! ***\n";
            cout << "     Omul a fost spanzurat! Mai incearca o data!\n";
        }
        
        cout << "\n========================================================\n\n";
        
        // Intrebare: Vrei sa joci din nou? - VALIDATA
        cout << "VREI SA JOCI DIN NOU? (DA/NU)\n";
        char response;
        
        bool validResponse = false;
        while (!validResponse) {
            if (cin >> response) {
                clearInputBuffer();
                response = toupper(response);
                
                if (response == 'D' || response == 'N') {
                    validResponse = true;
                } else {
                    cout << "EROARE! Introduce DA sau NU: ";
                }
            } else {
                cout << "EROARE! Input invalid. Introduce DA sau NU: ";
                clearInputBuffer();
            }
        }
        
        if (response == 'D') {
            playAgain = true;
            roundNumber++;
            cout << "\n";
        } else {
            playAgain = false;
            cout << "\nMULTUMESC CA AI JUCAT! LA REVEDERE!\n";
            cout << "========================================================\n\n";
        }
    }
    
    return 0;
}
