#include <SFML/Graphics.hpp>
#include <iostream>

// --- Controls for both players ---
const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,   // Player1 UP
    sf::Keyboard::Z,   // Player1 DOWN
    sf::Keyboard::Up,  // Player2 UP (manual mode)
    sf::Keyboard::Down // Player2 DOWN (manual mode)
};

// --- Game parameters ---
const sf::Vector2f paddleSize(25.f, 100.f);   // Paddle width & height
const float ballRadius = 10.f;                // Ball radius
const int gameWidth = 800;                    // Window width
const int gameHeight = 600;                   // Window height
const float paddleSpeed = 400.f;              // Player paddle movement speed
const float paddleOffSetWall = 10.f;          // Distance paddles from vertical walls
const float initialVelocityX = 100.f;         // Ball horizontal speed at serve
const float initialVelocityY = 60.f;          // Ball vertical speed at serve
const float velocityMultiplier = 1.1f;        // Speed-up factor on bounce

// --- AI settings ---
bool useAI = false;                 // AI toggle (press T to switch)
const float aiSpeedMultiplier = 0.95f;  // AI speed (slightly weaker than player)
const float aiReactionDelay = 0.15f;    // AI reaction delay in seconds
float aiTimer = 0.f;                    // Timer to track AI reaction

// --- Game objects ---
sf::CircleShape ball;
sf::RectangleShape paddles[2];

// --- Ball velocity & serving state ---
sf::Vector2f ballVelocity;
bool isPlayer1Serving = true;

// --- Score tracking ---
int scoreP1 = 0;
int scoreP2 = 0;

// --- Font and text for displaying score ---
sf::Font font;
sf::Text scoreText;

// --- Reset game state (after score or new game) ---
void reset() {
    // Reset paddles to center
    paddles[0].setPosition(paddleOffSetWall + paddleSize.x / 2.f, gameHeight / 2.f);
    paddles[1].setPosition(gameWidth - paddleOffSetWall - paddleSize.x / 2.f, gameHeight / 2.f);

    // Reset ball to center
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

    // Ball serves left or right depending on last scorer
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };

    // Update score display
    scoreText.setString(std::to_string(scoreP1) + " : " + std::to_string(scoreP2));

    // Keep score centered at the top
    scoreText.setPosition((gameWidth * 0.5f) - (scoreText.getLocalBounds().width * 0.5f), 10.f);
}

// --- Init function (run once at the start) ---
void init() {
    // Setup paddles
    for (sf::RectangleShape& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);  // origin at center for easier movement
    }

    // Setup ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius); // origin at center of circle

    // Load font for score text
    if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
        std::cerr << "Error: Could not load font!\n";
    }

    // Setup score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    // Reset game state (positions + velocity + score text)
    reset();
}

// --- Update function (game logic, runs every frame) ---
void update(float dt) {
    // --- Player 1 movement ---
    float direction1 = 0.f;
    if (sf::Keyboard::isKeyPressed(controls[0])) direction1--;
    if (sf::Keyboard::isKeyPressed(controls[1])) direction1++;
    paddles[0].move(0.f, direction1 * paddleSpeed * dt);

    // --- Player 2 movement ---
    static float aiDirection = 0.f; // remember last chosen direction

    if (useAI)
    {
        // Update AI timer
        aiTimer += dt;

        // Every aiReactionDelay seconds, re-evaluate ball position
        if (aiTimer >= aiReactionDelay)
        {
            if (ball.getPosition().y < paddles[1].getPosition().y)
                aiDirection = -1.f; // move up
            else if (ball.getPosition().y > paddles[1].getPosition().y)
                aiDirection = 1.f;  // move down
            else
                aiDirection = 0.f;  // stay still

            aiTimer = 0.f; // reset timer
        }

        // Keep moving in the last chosen direction
        paddles[1].move(0.f, aiDirection * paddleSpeed * dt * aiSpeedMultiplier);
    
    }
    else 
    {
        // Manual control (Player 2 uses Up/Down keys)
        float direction2 = 0.f;
        if (sf::Keyboard::isKeyPressed(controls[2])) direction2--;
        if (sf::Keyboard::isKeyPressed(controls[3])) direction2++;
        paddles[1].move(0.f, direction2 * paddleSpeed * dt);
    }

    // --- Keep paddles inside the screen ---
    for (sf::RectangleShape& p : paddles) 
    {
        if (p.getPosition().y - paddleSize.y / 2.f < 0)
            p.setPosition(p.getPosition().x, paddleSize.y / 2.f);
        if (p.getPosition().y + paddleSize.y / 2.f > gameHeight)
            p.setPosition(p.getPosition().x, gameHeight - paddleSize.y / 2.f);
    }

    // --- Move ball ---
    ball.move(ballVelocity * dt);

    // Ball position
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;

    // --- Collisions ---
    // Bottom wall
    if (by > gameHeight) {
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(sf::Vector2f(0.f, -10.f)); // push back inside
    }
    // Top wall
    else if (by < 0) {
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(sf::Vector2f(0.f, 10.f));
    }
    // Right wall (Player 1 scores)
    else if (bx > gameWidth) {
        scoreP1++;
        isPlayer1Serving = false;
        reset();
    }
    // Left wall (Player 2 scores)
    else if (bx < 0) {
        scoreP2++;
        isPlayer1Serving = true;
        reset();
    }

    // --- Paddle collisions ---
    // Left paddle
    else if (
        bx < paddleSize.x + paddleOffSetWall &&
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5f)
        ) {
        ballVelocity.x *= -velocityMultiplier;
        ball.move(sf::Vector2f(10.f, 0.f));
    }
    // Right paddle
    else if (
        bx > gameWidth - paddleSize.x - paddleOffSetWall &&
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5f)
        ) {
        ballVelocity.x *= -velocityMultiplier;
        ball.move(sf::Vector2f(-10.f, 0.f));
    }
}

// --- Render function (draws everything) ---
void render(sf::RenderWindow& window) {
    window.draw(ball);
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(scoreText); // draw score on top
}

// --- Main function ---
int main() {
    sf::RenderWindow window(sf::VideoMode({ gameWidth, gameHeight }), "PONG");
    window.setVerticalSyncEnabled(true); // enable vsync for smooth gameplay

    init(); // setup everything

    sf::Clock clock; // timer for delta time

    while (window.isOpen()) {
        // Handle events (close button, etc.)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Toggle AI with the T key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T) {
                useAI = !useAI;
                std::cout << "AI toggled: " << (useAI ? "ON" : "OFF") << std::endl;
            }
        }

        // Calculate delta time
        float dt = clock.restart().asSeconds();

        // Game logic
        update(dt);

        // Render everything
        window.clear();
        render(window);
        window.display();
    }

    return 0;
}
