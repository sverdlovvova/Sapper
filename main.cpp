#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

int main() {
    std::fstream file("saved_game.txt");
    std::srand(std::time(nullptr));

    int n, m, k;
    std::vector<std::vector<int>> field;
    std::vector<std::vector<int>> opens;

    char yn = 'N';
    if (file) {
        std::cout << "Do you want to continue the previous game? [Y/N]" << std::endl;
        std::cin >> yn;
    }
    if (yn == 'Y') {
        file >> n >> m >> k;
        field.assign(n + 2, std::vector<int> (m + 2, 0));
        opens.assign(n + 2, std::vector<int> (m + 2, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                int x;
                file >> x;
                if ((i + j) % 2 == x % 2) {
                    field[i][j] = 0;
                } else {
                    field[i][j] = 1;
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                file >> opens[i][j];
            }
        }
    } else {
        std::cout << "Enter field size and number of bombs:" << std::endl;
        std::cin >> n >> m >> k;
        while (k >= n * m / 2) {
            std::cout << "Too many bombs. Number of bombs must be less than half of number of cells. Choose different. Enter field size and a number of bombs again:" << std::endl;
            std::cin >> n >> m >> k;
        }
        field.assign(n + 2, std::vector<int> (m + 2, 0));
        opens.assign(n + 2, std::vector<int> (m + 2, 0));
        for (int i = 0; i < n; ++i) {
            field[i][0] = field[i][m + 1] = field[0][i] = field[n + 1][i] = 1;
        }
        //k = (std::rand() % 4) + 2;
        while (k > 0) {
            int x = std::rand() % 5 + 1;
            int y = std::rand() % 5;
            if (field[x][y]) {
                continue;
            }
            if (field[x - 1][y - 1] * field[x - 1][y] * field[x - 1][y + 1] * field[x][y + 1] * field[x + 1][y + 1] * field[x + 1][y] * field[x + 1][y - 1] * field[x][y - 1] == 0) {
                field[x][y] = 1;
                k--;
            }
        }
        for (int i = 0; i < n; ++i) {
            field[i][0] = field[i][m + 1] = field[0][i] = field[n + 1][i] = 0;
        }
    }

    file.clear();

    while (true) {
        std::cout << "Enter command:" << std::endl;
        std::string s;
        std::cin >> s;
        int x, y;
        std::string action;
        if ('0' <= s[0] && s[0] <= '9') {
            x = stoi(s);
            std::cin >> y >> action;
        } else {
            file << n << ' ' << m << ' ' << k << std::endl;
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= m; ++j) {
                    file << field[i][j] << ' ';
                }
                file << std::endl;
            }
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= m; ++j) {
                    file << (opens[i][j] + i + j) % 2 << ' ';
                }
                file << std::endl;
            }
            break;
        }
        if (x < 1 || x > n || y < 1 || y > m) {
            std::cout << "Wrong answer. Choose a cell within the bounds of the field" << std::endl;
        }
        if (action == "Flag") {
            if (opens[x][y] == 1) {
                std::cout << "Wrong command: the cell is already open" << std::endl;
            } else {
                std::cout << "Ok" << std::endl;
            }
            if (opens[x][y] == 0) {
                opens[x][y] = 2;
            }
        }
        if (action == "Open") {
            if (field[x][y]) {
                std::cout << "You lose" << std::endl;
                file.clear();
                break;
            } else {
                opens[x][y] = 1;
                std::cout << "Ok" << std::endl;
            }
        }
        int cnt_not_open = 0, cnt_bombs = 0;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (opens[i][j] == 0) {
                    std::cout << "*";
                    cnt_not_open++;
                } else if (opens[i][j] == 1) {
                    std::cout << field[i - 1][j - 1] + field[i - 1][j] + field[i - 1][j + 1] + field[i][j + 1] +
                    field[i + 1][j + 1] + field[i + 1][j] + field[i + 1][j - 1] + field[i][j - 1];
                } else if (opens[i][j] == 2) {
                    std::cout << "F";
                    cnt_bombs++;
                }
            }
            std::cout << std::endl;
        }
        if (cnt_not_open == 0 && cnt_bombs == k) {
            std::cout << "You win" << std::endl;
            file.clear();
            break;
        }
    }
}

