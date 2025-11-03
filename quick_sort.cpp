#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

const int WIDTH = 800;
const int HEIGHT = 600;
const int BAR_WIDTH = 8;
const int NUM_BARS = WIDTH / BAR_WIDTH;
const int DELAY = 10; // temps entre étapes

void drawBars(sf::RenderWindow& window, const std::vector<float>& values, int low = -1, int high = -1, int pivot = -1) {
    window.clear(sf::Color::Black);
    for (int i = 0; i < NUM_BARS; ++i) {
        sf::RectangleShape bar(sf::Vector2f(BAR_WIDTH - 1, -values[i]));
        bar.setPosition(i * BAR_WIDTH, HEIGHT);

        if (i == pivot)
            bar.setFillColor(sf::Color::Red);  // pivot
        else if (i >= low && i <= high)
            bar.setFillColor(sf::Color::Green); // plage actuelle
        else
            bar.setFillColor(sf::Color(100, 100, 255)); // barres non triées

        window.draw(bar);
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
}

int partition(std::vector<float>& values, int low, int high, sf::RenderWindow& window) {
    float pivot = values[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (values[j] < pivot) {
            i++;
            std::swap(values[i], values[j]);
        }
        drawBars(window, values, low, high, high);
    }

    std::swap(values[i + 1], values[high]);
    drawBars(window, values, low, high, high);
    return i + 1;
}

void quickSort(std::vector<float>& values, int low, int high, sf::RenderWindow& window) {
    if (low < high) {
        int pi = partition(values, low, high, window);
        quickSort(values, low, pi - 1, window);
        quickSort(values, pi + 1, high, window);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sorting Visualizer - Quick Sort (Souleymane)");

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::vector<float> values(NUM_BARS);
    for (int i = 0; i < NUM_BARS; ++i)
        values[i] = static_cast<float>(std::rand() % HEIGHT);

    bool sorted = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !sorted) {
                quickSort(values, 0, NUM_BARS - 1, window);
                sorted = true;
            }
        }

        drawBars(window, values);
    }

    return 0;
}
