// thread safe

#ifndef ILRD_NCURSES_HPP
#define ILRD_NCURSES_HPP

#include <string> // std::string
#include <mutex>  // std::mutex

namespace ilrd {

class NCurses {
public:
    explicit NCurses();
    ~NCurses();

    int get_rows() const;
    int get_cols() const;

    void println(const std::string& str, int row = 0, int col = 0);

    std::string get_line(int row = 0, int col = 0);
    std::string get_line(const std::string& msg, int row = 0, int col = 0);

    void move(int row = 0, int col = 0);

    NCurses(NCurses&&) = default;
    NCurses(const NCurses&) = default;
    NCurses& operator=(NCurses&&) = default;
    NCurses& operator=(const NCurses&) = default;

private:
    int m_max_rows;
    int m_max_cols;

    std::mutex m_lock;
};

} // namespace ilrd

#endif // ILRD_NCURSES_HPP
