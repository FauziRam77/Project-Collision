#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <cmath>

const int WINDOW_W = 1280;
const int WINDOW_H = 720;
const int MAX_BALLS = 10000;
const int INITIAL_BALLS = 1500;
const float MIN_SPEED = 1.0f;
const float MAX_SPEED = 2.5f;
const float RADIUS = 4.0f;

std::mt19937 rng((unsigned)std::random_device{}());
std::uniform_real_distribution<float> uni01(0.f, 1.f);

struct Ball {
    sf::CircleShape shape;
    sf::Vector2f vel;
};

sf::Color randomColor() {
    std::uniform_int_distribution<int> d(30, 230);
    return sf::Color((sf::Uint8)d(rng), (sf::Uint8)d(rng), (sf::Uint8)d(rng));
}

Ball makeBall(float speed) {
    std::uniform_real_distribution<float> px(RADIUS, WINDOW_W - RADIUS);
    std::uniform_real_distribution<float> py(RADIUS, WINDOW_H - RADIUS);
    std::uniform_real_distribution<float> ang(0.f, 2.f * 3.14159265f);

    Ball b;
    b.shape.setRadius(RADIUS);
    b.shape.setOrigin(RADIUS, RADIUS);
    b.shape.setPosition(px(rng), py(rng));
    b.shape.setFillColor(randomColor());

    float a = ang(rng);
    b.vel = { std::cos(a) * speed, std::sin(a) * speed };
    return b;
}

class QuadTree {
public:
    sf::FloatRect rect;
    int capacity;
    int depth;
    int maxDepth;
    float minSize;
    std::vector<Ball*> items;
    bool divided = false;

    QuadTree *nw=nullptr, *ne=nullptr, *sw=nullptr, *se=nullptr;

    QuadTree(const sf::FloatRect& r, int cap=6, int d=0, int maxD=10, float minS=6.f)
        : rect(r), capacity(cap), depth(d), maxDepth(maxD), minSize(minS) {}

    ~QuadTree() {
        delete nw; delete ne; delete sw; delete se;
    }

    bool tooSmall() const {
        return rect.width <= minSize || rect.height <= minSize;
    }

    void subdivide() {
        float x = rect.left;
        float y = rect.top;
        float w = rect.width / 2.f;
        float h = rect.height / 2.f;

        nw = new QuadTree({x,     y,     w, h}, capacity, depth+1, maxDepth, minSize);
        ne = new QuadTree({x + w, y,     w, h}, capacity, depth+1, maxDepth, minSize);
        sw = new QuadTree({x,     y + h, w, h}, capacity, depth+1, maxDepth, minSize);
        se = new QuadTree({x + w, y + h, w, h}, capacity, depth+1, maxDepth, minSize);

        divided = true;
    }

    bool insert(Ball* b) {
        if (!rect.intersects(b->shape.getGlobalBounds())) return false;
        if ((int)items.size() < capacity || depth >= maxDepth || tooSmall()) {
            items.push_back(b);
            return true;
        }
        if (!divided) subdivide();
        if (nw->insert(b)) return true;
        if (ne->insert(b)) return true;
        if (sw->insert(b)) return true;
        if (se->insert(b)) return true;
        return false;
    }

    void query(const sf::FloatRect &range, std::vector<Ball*> &out) {
        if (!rect.intersects(range)) return;

        for (auto *b : items)
            if (range.intersects(b->shape.getGlobalBounds()))
                out.push_back(b);

        if (!divided) return;

        nw->query(range, out);
        ne->query(range, out);
        sw->query(range, out);
        se->query(range, out);
    }

    sf::Color depthColor() const {
        int g = std::min(40 + depth * 20, 255);
        return sf::Color(0, g, 0, 130);
    }

    void draw(sf::RenderWindow& win) {
        sf::RectangleShape r;
        r.setPosition(rect.left, rect.top);
        r.setSize({rect.width, rect.height});
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(depthColor());
        r.setOutlineThickness(1);
        win.draw(r);

        if (!divided) return;
        nw->draw(win);
        ne->draw(win);
        sw->draw(win);
        se->draw(win);
    }
};

void collideSwap(Ball& A, Ball& B) {
    sf::Vector2f pA = A.shape.getPosition();
    sf::Vector2f pB = B.shape.getPosition();
    float dx = pB.x - pA.x, dy = pB.y - pA.y;
    float d2 = dx*dx + dy*dy;
    float minDist = (2.f * RADIUS);
    if (d2 >= minDist*minDist || d2 == 0.f) return;

    float d = std::sqrt(d2);
    sf::Vector2f n = { dx/d, dy/d };
    sf::Vector2f rv = { B.vel.x - A.vel.x, B.vel.y - A.vel.y };
    float rel = rv.x * n.x + rv.y * n.y;
    if (rel > 0) return;

    float j = -rel;
    A.vel.x -= j * n.x;
    A.vel.y -= j * n.y;
    B.vel.x += j * n.x;
    B.vel.y += j * n.y;

    float overlap = 0.5f * (minDist - d);
    A.shape.move(-n.x * overlap, -n.y * overlap);
    B.shape.move( n.x * overlap,  n.y * overlap);
}

int main(){
    sf::RenderWindow window({WINDOW_W, WINDOW_H}, "Particles - BruteForce / QuadTree");
    window.setFramerateLimit(0);

    int ballCount = INITIAL_BALLS;
    float speed = 1.0f;
    bool useQuad = true;

    std::vector<Ball> balls;
    balls.reserve(MAX_BALLS);
    for (int i=0;i<ballCount;i++) balls.push_back(makeBall(speed));

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        font.loadFromFile("arial.ttf");

    double fps = 0;
    sf::Clock clock;

    while (window.isOpen()){
        sf::Event ev;
        while (window.pollEvent(ev)){
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed){
                if (ev.key.code == sf::Keyboard::Num1) useQuad = false;
                if (ev.key.code == sf::Keyboard::Num2) useQuad = true;
                if (ev.key.code == sf::Keyboard::Up) {
                    int add = 50;
                    for (int k=0;k<add && (int)balls.size() < MAX_BALLS; ++k)
                        balls.push_back(makeBall(speed));
                }
                if (ev.key.code == sf::Keyboard::Down) {
                    int rem = 50;
                    for (int k=0;k<rem && !balls.empty(); ++k) balls.pop_back();
                }
                if (ev.key.code == sf::Keyboard::Right) {
                    speed += 0.05f;
                    if (speed > MAX_SPEED) speed = MAX_SPEED;
                    for (auto &b: balls) {
                        float len = std::sqrt(b.vel.x*b.vel.x + b.vel.y*b.vel.y);
                        if (len > 0) b.vel = (b.vel / len) * speed;
                    }
                }
                if (ev.key.code == sf::Keyboard::Left) {
                    speed -= 0.05f;
                    if (speed < MIN_SPEED) speed = MIN_SPEED;
                    for (auto &b: balls) {
                        float len = std::sqrt(b.vel.x*b.vel.x + b.vel.y*b.vel.y);
                        if (len > 0) b.vel = (b.vel / len) * speed;}
                }
            }
        }

        for (auto &b : balls) {
            sf::Vector2f p = b.shape.getPosition();
            if (p.x - RADIUS <= 0 && b.vel.x < 0) b.vel.x = -b.vel.x;
            if (p.x + RADIUS >= WINDOW_W && b.vel.x > 0) b.vel.x = -b.vel.x;
            if (p.y - RADIUS <= 0 && b.vel.y < 0) b.vel.y = -b.vel.y;
            if (p.y + RADIUS >= WINDOW_H && b.vel.y > 0) b.vel.y = -b.vel.y;
            b.shape.move(b.vel);
        }

        if (!useQuad) {
            int n = (int)balls.size();
            for (int i=0;i<n;i++)
                for (int j=i+1;j<n;j++)
                    collideSwap(balls[i], balls[j]);
        } else {
            QuadTree qt({0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H}, 6);
            for (auto &b : balls) qt.insert(&b);

            for (auto &b : balls) {
                sf::FloatRect range = b.shape.getGlobalBounds();
                range.left -= 1; range.top -= 1; range.width += 2; range.height += 2;
                std::vector<Ball*> found;
                qt.query(range, found);
                for (auto *o : found) {
                    if (&b == o) continue;
                    if (&b < o) collideSwap(b, *o);
                }
            }
        }

        fps = 1.0 / clock.restart().asSeconds();
        window.clear(sf::Color::Black);
        if (useQuad) {
            QuadTree qt_draw({0.f,0.f,(float)WINDOW_W,(float)WINDOW_H}, 6);
            for (auto &b: balls) qt_draw.insert(&b);
            qt_draw.draw(window);
        }
        for (auto &b: balls) window.draw(b.shape);
        sf::Text top;
        top.setFont(font);
        top.setCharacterSize(16);
        top.setFillColor(sf::Color::White);
        top.setPosition(10, 8);
        top.setString("Mode: " + std::string(useQuad ? "Quadtree (2)" : "BruteForce (1)") +
                      "\nFPS: " + std::to_string((int)fps));
        window.draw(top);

        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(16);
        info.setFillColor(sf::Color::White);
        info.setPosition(10, WINDOW_H - 70);
        info.setString(
            "Balls: " + std::to_string((int)balls.size()) + " / " + std::to_string(MAX_BALLS) +
            "\nSpeed: " + std::to_string((float)round(speed*100)/100.0f) +
            "\nControls: 1/2 = mode, Up/Down = +50/-50 balls, Left/Right = speed"
        );
        window.draw(info);

        window.display();
    }

    return 0;
}