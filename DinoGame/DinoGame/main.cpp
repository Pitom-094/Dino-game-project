#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>

using namespace sf;
using namespace std;

const int windowWidth = 800;
const int windowHeight = 600;
const float gravity = 0.5f;
const float jumpSpeed = -10.0f;
const float groundOffset = 50.0f;
const float dinoOffset = 20.0f;
const float obstacleOffset = 20.0f;

int main() {
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Dino Game");
    window.setFramerateLimit(60);

    Texture dinoTexture;
    if (!dinoTexture.loadFromFile("dino.png")) {
        cout << "Error loading dino image!" << endl;
        return -1;
    }

    Sprite dino;
    dino.setTexture(dinoTexture);
    dino.setScale(0.1f, 0.1f);
    dino.setPosition(200.0f, windowHeight - groundOffset - dino.getGlobalBounds().height - dinoOffset);

    Texture groundTexture;
    if (!groundTexture.loadFromFile("ground.png")) {
        cout << "Error loading ground image!" << endl;
        return -1;
    }

    Sprite ground1, ground2;
    ground1.setTexture(groundTexture);
    ground1.setScale(static_cast<float>(windowWidth) / groundTexture.getSize().x, 1.0f);
    ground1.setPosition(0.0f, windowHeight - groundTexture.getSize().y - groundOffset);

    ground2.setTexture(groundTexture);
    ground2.setScale(static_cast<float>(windowWidth) / groundTexture.getSize().x, 1.0f);
    ground2.setPosition(ground1.getGlobalBounds().width, windowHeight - groundTexture.getSize().y - groundOffset);

    Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile("cactus.png")) {
        cout << "Error loading obstacle image!" << endl;
        return -1;
    }

    vector<Sprite> obstacles;
    int numObstacles = 2;
    float obstacleSpeed = 4.0f;
    float initialXPositions[] = {windowWidth + 200, windowWidth + 500};

    for (int i = 0; i < numObstacles; i++) {
        Sprite obstacle;
        obstacle.setTexture(obstacleTexture);
        obstacle.setScale(0.06f, 0.06f);
        obstacle.setPosition(initialXPositions[i], windowHeight - groundOffset - obstacle.getGlobalBounds().height - obstacleOffset);
        obstacles.push_back(obstacle);
    }

    float dinoYVelocity = 0.0f;
    bool isJumping = false;
    bool isPaused = false;
    int score = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
                isPaused = !isPaused;
            }
        }

        if (!isPaused) {
            if (Keyboard::isKeyPressed(Keyboard::Space) && !isJumping) {
                dinoYVelocity = jumpSpeed;
                isJumping = true;
            }

            dino.move(0, dinoYVelocity);
            dinoYVelocity += gravity;

            if (dino.getPosition().y + dino.getGlobalBounds().height > windowHeight - groundOffset - dinoOffset) {
                dino.setPosition(dino.getPosition().x, windowHeight - groundOffset - dino.getGlobalBounds().height - dinoOffset);
                dinoYVelocity = 0.0f;
                isJumping = false;
            }

            ground1.move(-obstacleSpeed, 0.0f);
            ground2.move(-obstacleSpeed, 0.0f);

            if (ground1.getPosition().x + ground1.getGlobalBounds().width < 0) {
                ground1.setPosition(ground2.getPosition().x + ground2.getGlobalBounds().width, ground1.getPosition().y);
            }
            if (ground2.getPosition().x + ground2.getGlobalBounds().width < 0) {
                ground2.setPosition(ground1.getPosition().x + ground1.getGlobalBounds().width, ground2.getPosition().y);
            }

            for (auto& obstacle : obstacles) {
                obstacle.move(-obstacleSpeed, 0.0f);

                if (obstacle.getPosition().x + obstacle.getGlobalBounds().width < 0) {
                    obstacle.setPosition(windowWidth, windowHeight - groundOffset - obstacle.getGlobalBounds().height - obstacleOffset);
                    score++;
                    cout << "Score: " << score << endl;

                    if (score % 5 == 0) {
                        obstacleSpeed += 0.25f;
                        cout << "Speed increased: " << obstacleSpeed << endl;
                    }
                }

                if (dino.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
                    cout << "Game Over! Final Score: " << score << endl;
                    window.close();
                }
            }
        }

        window.clear(Color::Black);
        window.draw(ground1);
        window.draw(ground2);
        window.draw(dino);
        for (const auto& obstacle : obstacles) {
            window.draw(obstacle);
        }

        window.display();
    }

    return 0;
}
