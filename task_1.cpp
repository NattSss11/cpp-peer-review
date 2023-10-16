
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class EBook {
public:
    EBook()
        : _users(MAX_USERS_COUNT + 1, -1)
        , _pages(MAX_PAGES_COUNT, 0)
    {}

    void Read(int user, int page) {
        int current_page = _users[user] + 1;
        while (current_page <= page) {
            ++_pages[current_page];
            ++current_page;
        }
        _users[user] = page;
    }

    double Cheer(int user) {
        if (_users[user] == -1)
            return 0;
        if (_pages[0] == 1)
            return 1;
        int pages_read = _pages[0] - _pages[_users[user]];
        return static_cast<double>(pages_read) / (_pages[0] - 1);
    }

    void ReadEBook(istream& input, ostream& output ) {
        size_t count;
        input >> count;
        for (size_t i = 0; i < count; ++i) {
            string request;
            int user;
            input >> request >> user;
            if (request == "READ") {
                int page;
                input >> page;
                Read(user, page);
            } else if (request == "CHEER") {
                output << setprecision(6) << Cheer(user) << endl;
            }
        }
    }

private:
    vector<int> _users;
    vector<int> _pages;

    static const int MAX_USERS_COUNT = 100000;
    static const int MAX_PAGES_COUNT = 1000;
};

int main() {
    EBook ebook;
    ebook.ReadEBook(cin,cout);
}
