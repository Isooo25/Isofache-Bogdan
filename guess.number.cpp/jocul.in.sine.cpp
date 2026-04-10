#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    int x, inc = 10;

    srand(time(0));
    int nr = rand() % 100 + 1;

    while (inc > 0)
    {
        cout << "Incercari ramase: " << inc << endl;
        cout << "Ghiceste numarul (1-100): ";
        cin >> x;

        if (x == nr)
        {
            cout << "Corect! Ai castigat!" << endl;
            inc = 0; // oprește bucla
        }
        else if (x < nr)
        {
            cout << "Mai mare!" << endl;
            inc--;
        }
        else
        {
            cout << "Mai mic!" << endl;
            inc--;
        }
    }

    if (x != nr)
    {
        cout << "Ai pierdut! Numarul era: " << nr << endl;
    }

    system("pause");
    return 0;
}