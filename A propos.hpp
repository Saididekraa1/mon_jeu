#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

class Scenario {
public:
    sf::RenderWindow& window;
    sf::Font& font;

    // Background (SFML 3 => no default constructor)
    std::optional<sf::Texture> bgTexture;
    std::optional<sf::Sprite> background;

    // Overlay sombre
    sf::RectangleShape overlay;

    std::string fullText =
        "2047. La compagnie OUR WORLD domine le monde du travail.\n\n"
        "Ses employes vivent sous des regles strictes,\n"
        "surveilles chaque seconde de leur vie.\n\n"
        "Personne ne questionne. Personne ne s'echappe.\n\n"
        "Jusqu'au jour ou TU decouvres la verite...\n\n"
        "La matrice existe. Et OUR WORLD le sait.\n\n"
        "Maintenant ils te suivent.\n\n"
        "C'est ta derniere chance.";

    std::string displayedText = "";

    std::optional<sf::Text> storyText;
    std::optional<sf::Text> skipText;
    std::optional<sf::Text> continueText;
    std::optional<sf::Text> titleText;

    // Typewriter
    float timer = 0.f;
    float charDelay = 0.04f;
    int charIndex = 0;
    bool finished = false;

    // UI
    sf::RectangleShape headerLine;

    sf::RectangleShape btnContinue;
    bool showButton = false;

    Scenario(sf::RenderWindow& win, sf::Font& f)
        : window(win), font(f)
    {
        // TEXT INIT (SFML 3 safe)
        titleText.emplace(font, "A propos", 20);
        storyText.emplace(font, "", 20);
        skipText.emplace(font, "[ PASSER ]", 16);
        continueText.emplace(font, "CONTINUER  >", 22);

        setup();
    }

    void setup()
    {
        // Background texture
        bgTexture.emplace();

        if (!bgTexture->loadFromFile("images/background2.jpg"))
            bgTexture->loadFromFile("images/background.png");

        background.emplace(*bgTexture);

        sf::Vector2u s = bgTexture->getSize();
        background->setScale({1000.f / s.x, 600.f / s.y});

        // Overlay
        overlay.setSize({1000.f, 600.f});
        overlay.setFillColor(sf::Color(0, 5, 20, 200));

        // Header line
        headerLine.setSize({1000.f, 2.f});
        headerLine.setFillColor(sf::Color(60, 140, 220, 180));
        headerLine.setPosition({0.f, 60.f});

        // Title
        titleText->setFillColor(sf::Color(180, 220, 255));
        titleText->setStyle(sf::Text::Bold);
        titleText->setPosition({30.f, 18.f});

        // Skip
        skipText->setFillColor(sf::Color(100, 150, 200));
        skipText->setPosition({870.f, 22.f});

        // Story
        storyText->setFillColor(sf::Color(200, 230, 255));
        storyText->setLineSpacing(1.6f);
        storyText->setPosition({80.f, 90.f});

        // Button
        btnContinue.setSize({220.f, 50.f});
        btnContinue.setFillColor(sf::Color(0, 0, 0, 130));
        btnContinue.setPosition({780.f, 530.f});

        continueText->setFillColor(sf::Color::White);
        continueText->setStyle(sf::Text::Bold);

        sf::FloatRect cb = continueText->getLocalBounds();
        continueText->setOrigin({cb.position.x + cb.size.x / 2.f,
                                 cb.position.y + cb.size.y / 2.f});

        continueText->setPosition({780.f + 110.f, 530.f + 25.f});
    }

    void reset()
    {
        displayedText = "";
        charIndex = 0;
        timer = 0.f;
        finished = false;
        showButton = false;
        storyText->setString("");
    }

    void update(float dt)
    {
        if (finished) return;

        timer += dt;

        if (timer >= charDelay && charIndex < (int)fullText.size())
        {
            timer = 0.f;
            displayedText += fullText[charIndex];
            charIndex++;
            storyText->setString(displayedText);
        }

        if (charIndex >= (int)fullText.size())
        {
            finished = true;
            showButton = true;
        }
    }

    void skipTypewriter()
    {
        displayedText = fullText;
        storyText->setString(displayedText);
        charIndex = (int)fullText.size();
        finished = true;
        showButton = true;
    }

    bool handleClick(sf::Vector2f mp)
    {
        if (skipText->getGlobalBounds().contains(mp))
        {
            skipTypewriter();
            return false;
        }

        if (showButton && btnContinue.getGlobalBounds().contains(mp))
            return true;

        return false;
    }

    void drawButton()
    {
        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);
        bool hovered = btnContinue.getGlobalBounds().contains(mp);

        sf::Color borderColor = hovered ? sf::Color(180, 220, 255) : sf::Color::White;
        btnContinue.setFillColor(hovered ? sf::Color(0, 80, 160, 160)
                                          : sf::Color(0, 0, 0, 130));

        window.draw(btnContinue);

        float x = btnContinue.getPosition().x;
        float y = btnContinue.getPosition().y;
        float w = btnContinue.getSize().x;
        float h = btnContinue.getSize().y;

        float cs = 10.f, th = 2.f;

        auto drawLine = [&](float lx, float ly, float lw, float lh)
        {
            sf::RectangleShape l({lw, lh});
            l.setFillColor(borderColor);
            l.setPosition({lx, ly});
            window.draw(l);
        };

        drawLine(x, y, cs, th);          drawLine(x, y, th, cs);
        drawLine(x + w - cs, y, cs, th); drawLine(x + w - th, y, th, cs);
        drawLine(x, y + h - th, cs, th); drawLine(x, y + h - cs, th, cs);
        drawLine(x + w - cs, y + h - th, cs, th);
        drawLine(x + w - th, y + h - cs, th, cs);

        continueText->setFillColor(hovered ? sf::Color(180, 220, 255)
                                           : sf::Color::White);

        window.draw(*continueText);
    }

    void draw()
    {
        window.draw(*background);
        window.draw(overlay);
        window.draw(headerLine);
        window.draw(*titleText);
        window.draw(*skipText);
        window.draw(*storyText);

        if (showButton)
            drawButton();
    }
};
