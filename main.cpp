#include <SFML/Graphics.hpp>

int main(){
    const int W=800, H=600;
    sf::RenderWindow window(sf::VideoMode(W,H), "Bola Kok hijau");
    window.setFramerateLimit(60);

    sf::CircleShape ball(20.f);             // radius 20
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(100.f,100.f);

    float vx=3.f, vy=2.f;                   // velocity

    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)) if(e.type==sf::Event::Closed) window.close();

        ball.move(vx, vy);
        auto p = ball.getPosition();
        float r = ball.getRadius();
        if(p.x <= 0 || p.x + 2*r >= W) vx = -vx;   // bounce x
        if(p.y <= 0 || p.y + 2*r >= H) vy = -vy;   // bounce y

        window.clear(sf::Color::Black);
        window.draw(ball);
        window.display();
    }

    return 0;
}
