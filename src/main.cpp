#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

// enum Face {
//     U,
//     L,
//     F,
//     R,
//     B,
//     D,
// };

string const u_move = "U7U4U1U8U5U2U9U6U3"
                      "F1F2F3L4L5L6L7L8L9"
                      "R1R2R3F4F5F6F7F8F9"
                      "B1B2B3R4R5R6R7R8R9"
                      "L1L2L3B4B5B6B7B8B9"
                      "D1D2D3D4D5D6D7D8D9";

string const u_rev_move = "U3U6U9U2U5U8U1U4U7"
                          "B1B2B3L4L5L6L7L8L9"
                          "L1L2L3F4F5F6F7F8F9"
                          "F1F2F3R4R5R6R7R8R9"
                          "R1R2R3B4B5B6B7B8B9"
                          "D1D2D3D4D5D6D7D8D9";

string const d_move = "U1U2U3U4U5U6U7U8U9"
                      "L1L2L3L4L5L6B7B8B9"
                      "F1F2F3F4F5F6L7L8L9"
                      "R1R2R3R4R5R6F7F8F9"
                      "B1B2B3B4B5B6R7R8R9"
                      "D7D4D1D8D5D2D9D6D3";

string const d_rev_move = "U1U2U3U4U5U6U7U8U9"
                          "L1L2L3L4L5L6F7F8F9"
                          "F1F2F3F4F5F6R7R8R9"
                          "R1R2R3R4R5R6B7B8B9"
                          "B1B2B3B4B5B6L7L8L9"
                          "D3D6D9D2D5D8D1D4D7";

// map<char, Face> char_to_face = {
//     {'U', U},
//     {'L', L},
//     {'F', F},
//     {'R', R},
//     {'B', B},
//     {'D', D},
// };

class Puzzle {
public:
    void set_state(string const&& state) {
        m_state = move(state);
    }

    string const get_face(char f) const {
        int idx;
        switch (f) {
        case 'U':
            idx = 0;
            break;
        case 'L':
            idx = 1;
            break;
        case 'F':
            idx = 2;
            break;
        case 'R':
            idx = 3;
            break;
        case 'B':
            idx = 4;
            break;
        case 'D':
            idx = 5;
            break;
        }
        return string(m_state.begin() + idx * 9, m_state.begin() + idx * 9 + 9);
    }

    char const get_facelet(char f, int pos) const {
        return get_face(f)[pos];
    }

    void execute_move(string const& move) {
        string new_state = "";
        auto iter = move.begin();
        while (iter != move.end()) {
            auto face = *iter++;
            auto pos = *iter++ - '0';
            new_state += get_facelet(face, pos - 1);
        }
        m_state = new_state;

        cout << m_state << '\n';
    }

private:
    string m_state;
};

void draw_face(char f, sf::RenderWindow& window, Puzzle const& puzzle, sf::Vector2f pos) {
    auto face_state = puzzle.get_face(f);
    sf::Color color;

    int i = 0;
    for (auto sticker : face_state) {
        switch (sticker) {
        case 'U':
            color = sf::Color::White;
            break;
        case 'L':
            color = sf::Color::Green;
            break;
        case 'F':
            color = sf::Color::Red;
            break;
        case 'R':
            color = sf::Color::Blue;
            break;
        case 'B':
            color = sf::Color(0xff8000ff);
            break;
        case 'D':
            color = sf::Color::Yellow;
            break;
        default:
            color = sf::Color::Black;
            break;
        }
        auto x = i % 3, y = i / 3;
        sf::RectangleShape rect({75, 75});
        rect.setFillColor(color);
        rect.setPosition(pos + sf::Vector2f(x * 80, y * 80));
        window.draw(rect);
        i++;
    }
}

vector<pair<char, sf::Vector2f>> const face_pos = {
    {'U', {245, 0}},
    {'L', {0, 245}},
    {'F', {245, 245}},
    {'R', {490, 245}},
    {'B', {735, 245}},
    {'D', {245, 490}},
};

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "Rubik" };

    Puzzle cube;
    cube.set_state("UUUUUUUUU"
                   "LLLLLLLLL"
                   "FFFFFFFFF"
                   "RRRRRRRRR"
                   "BBBBBBBBB"
                   "DDDDDDDDD");

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Key::U:
                    if (event.key.shift)
                        cube.execute_move(u_rev_move);
                    else
                        cube.execute_move(u_move);
                    break;
                case sf::Keyboard::Key::D:
                    if (event.key.shift)
                        cube.execute_move(d_rev_move);
                    else
                        cube.execute_move(d_move);
                    break;
                }
            }
        }

        window.clear();
        for (auto [f, pos] : face_pos) {
            draw_face(f, window, cube, pos);
        }
        window.display();
    }
}