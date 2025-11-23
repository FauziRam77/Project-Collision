#include <SFML/Graphics.hpp>

// g++ main.cpp -o example     -lsfml-graphics -lsfml-window -lsfml-system
// ./example


int main() {
    // Create a window (800x600)
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Example");

    // Circle shape
    sf::CircleShape circle(50.f);   // radius 50
    circle.setFillColor(sf::Color::White);
    circle.setPosition(100, 100);

    // Velocity
    sf::Vector2f velocity(2.0f, 1.5f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close on window X or ESC
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // Move circle
        sf::Vector2f pos = circle.getPosition();
        pos += velocity;

        // Bounce inside window
        if (pos.x < 0 || pos.x + circle.getRadius()*2 > 800) velocity.x *= -1;
        if (pos.y < 0 || pos.y + circle.getRadius()*2 > 600) velocity.y *= -1;

        circle.setPosition(pos);

        // Draw
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }


    return 0;
}
