#include <chrono>
#include <iostream>
#include <list>
#include <numeric>
#include <thread>
#include <vector>

const int grid_size{40};
const int interval{200}; // milliseconds

int live_neighbours(const int& row, const int& col,
                    const std::vector<std::vector<int>>& board);
void iterate(std::vector<std::vector<int>>& board, std::vector<std::vector<int>>& old_board);
void show_field(const std::vector<std::vector<int>>& board);
void update_field(std::list<int>& data, std::vector<std::vector<int>>& board);

template <typename T>
std::vector<T> subvector(const std::vector<T>& v, const int& m, const int& n) {
    auto first = v.cbegin() + m;
    auto last  = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
}

int main(int argc, char* argv[]) {
    auto clear_screen = []() {
        // clear screen
        std::cout << std::string(1000, '\n');
    };

    std::vector<std::vector<int>> field(grid_size, std::vector<int>(grid_size, 0));
    std::vector<std::vector<int>> field_prev{};

    // initiate field
    std::vector<std::list<int>> inital_state{{1, 25},
                                   {2, 23, 25},
                                   {3, 13, 14, 21, 22, 35, 36},
                                   {4, 12, 16, 21, 22, 35, 36},
                                   {5, 1, 2, 11, 17, 21, 22},
                                   {6, 1, 2, 11, 15, 17, 18, 23, 25},
                                   {7, 11, 17, 25},
                                   {8, 12, 16},
                                   {9, 13, 14}};

    for (std::list<int> row : inital_state) { update_field(row, field); }

    /* show_field(field); */
    /* return 0; */

    clear_screen();
    int i = 1;

    while (i > 0) {
        field_prev = field;
        show_field(field);
        std::cout << i << std::endl;
        iterate(field, field_prev);
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        clear_screen();

        ++i;
    }
}

void update_field(std::list<int>& data, std::vector<std::vector<int>>& board) {
    // data assumed to have at least 2 values
    // first value is the row and every other volue is the corresponding col

    int row = data.front();
    data.pop_front();

    for (auto col : data) { board[row][col] = 1; }
}

void show_field(const std::vector<std::vector<int>>& board) {
    // prints the field to stdout
    for (std::vector<int> row : board) {
        for (int value : row) {
            if (value == 1) {
                std::cout << "0";
            } else {
                std::cout << " ";
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }
}

void iterate(std::vector<std::vector<int>>& board, std::vector<std::vector<int>>& old_board) {
    /* RULES
     * Any live cell with two or three live neighbours survives.
     * Any dead cell with three live neighbours becomes a live cell.
     * All other live cells die in the next generation. Similarly, all other
     * dead cells stay dead.
     */

    for (int row{0}; row < grid_size; ++row) {
        for (int col{0}; col < grid_size; ++col) {
            int n = live_neighbours(row, col, old_board);
            int& old_cell{old_board[row][col]};
            int& new_cell{board[row][col]};

            if (old_cell == 1) {
                if ((n != 2) && (n != 3)) {
                    new_cell = 0;
                }
            }

            if (old_cell == 0) {
                if (n == 3) {
                    new_cell = 1;
                }
            }
        }
    }
}

int live_neighbours(const int& row, const int& col,
                    const std::vector<std::vector<int>>& board) {
    int neighbours = 0;
    const int m    = std::max(0, col - 1);
    const int n    = std::min(col + 1, grid_size);

    if (row != 0) {
        std::vector<int> row_above = *(board.begin() + row - 1);
        std::vector<int> buf_above = subvector(row_above, m, n);

        neighbours += accumulate(buf_above.begin(), buf_above.end(), 0);

        /* std::vector<int> row_above = board[row - 1]; */
        /* neighbours += accumulate(row_above.begin() + m, row_above.begin() +
         * n, 0); */
    }

    if (row != (grid_size - 1)) {
        std::vector<int> row_below = *(board.begin() + row + 1);
        std::vector<int> buf_below = subvector(row_below, m, n);

        neighbours += accumulate(buf_below.begin(), buf_below.end(), 0);

        /* std::vector<int> row_below = board[row + 1]; */
        /* neighbours += accumulate(row_below.begin() + m, row_below.begin() +
         * n, 0); */
    }

    std::vector<int> row_current = *(board.begin() + row);
    std::vector<int> buf_current = subvector(row_current, m, n);

    neighbours +=
        accumulate(buf_current.begin(), buf_current.end(), 0) - board[row][col];

    /* std::vector<int> row_current = board[row]; */
    /* neighbours += accumulate(row_current.begin() + m, row_current.begin() +
     * n, 0) - board[row][col]; */

    return neighbours;
}
