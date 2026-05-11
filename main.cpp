#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <optional>
#include "parametres.hpp"
#include "A propos.hpp"
#include "loading.hpp"
#include "jeu_page.hpp"

void drawStyledButton(sf::RenderWindow& window, sf::Text& txt, sf::RectangleShape& box, bool hovered)
{
    sf::Color borderColor = hovered ? sf::Color(180, 220, 255) : sf::Color::White;
    float x = box.getPosition().x;
    float y = box.getPosition().y;
    float w = box.getSize().x;
    float h = box.getSize().y;
    float cornerSize = 10.f;
    float thickness = 2.f;

    window.draw(box);

    auto drawCorner = [&](float cx, float cy) {
        sf::RectangleShape h_line({cornerSize, thickness});
        h_line.setFillColor(borderColor);
        h_line.setPosition({cx, cy});
        window.draw(h_line);

        sf::RectangleShape v_line({thickness, cornerSize});
        v_line.setFillColor(borderColor);
        v_line.setPosition({cx, cy});
        window.draw(v_line);
    };

    drawCorner(x, y);
    drawCorner(x + w - cornerSize, y);
    drawCorner(x, y + h - cornerSize);
    drawCorner(x + w - cornerSize, y + h - cornerSize);

    window.draw(txt);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 600}), "Last Chance Menu");

    // Background
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("images/background.png"))
    {
        std::cout << "Error loading background\n";
        return -1;
    }
    sf::Sprite background(bgTexture);
    background.setScale({
        1000.f / bgTexture.getSize().x,
        600.f / bgTexture.getSize().y
    });

    // Font
    sf::Font font;
    if (!font.openFromFile("fonts/framd.ttf"))
    {
        std::cout << "Error loading font\n";
        return -1;
    }

    // Music
    sf::Music music;
    if (!music.openFromFile("sounds/background-music.mp3"))
    {
        std::cout << "Error loading music\n";
        return -1;
    }
    music.setLooping(true);
    music.setVolume(50.f);
    music.play();

    // Init loading
    init_loading(font);

    // Init jeu
    init_jeu(font);

    // Pages
    enum class Page { MENU, PARAMETRES, SCENARIO, LOADING, JEU };
    Page currentPage = Page::MENU;

    // Pages objects
    Parametres parametres(window, font, music);
    Scenario scenario(window, font);

    // Clock
    sf::Clock clock;

    // Buttons
    float btnW = 220.f, btnH = 55.f;
    float btnX = 1000.f - btnW - 40.f;

    sf::Text jouer(font, "Jouer", 36);
    sf::Text param(font, "Parametres", 36);
    sf::Text scenarioBtn(font, "A propos", 36);
    sf::Text quitterBtn(font, "Quitter", 36);

    sf::RectangleShape boxJouer({btnW, btnH});
    sf::RectangleShape boxParam({btnW, btnH});
    sf::RectangleShape boxScenario({btnW, btnH});
    sf::RectangleShape boxQuitter({btnW, btnH});

    boxJouer.setPosition({btnX, 200.f});
    boxParam.setPosition({btnX, 290.f});
    boxScenario.setPosition({btnX, 380.f});
    boxQuitter.setPosition({btnX, 470.f});

    auto centerText = [&](sf::Text& txt, sf::RectangleShape& box) {
        sf::FloatRect tb = txt.getLocalBounds();
        txt.setOrigin(tb.position + tb.size / 2.f);
        txt.setPosition(box.getPosition() + box.getSize() / 2.f);
    };

    centerText(jouer, boxJouer);
    centerText(param, boxParam);
    centerText(scenarioBtn, boxScenario);
    centerText(quitterBtn, boxQuitter);

    for (auto* box : {&boxJouer, &boxParam, &boxScenario, &boxQuitter})
        box->setFillColor(sf::Color(0, 0, 0, 130));

    for (auto* txt : {&jouer, &param, &scenarioBtn, &quitterBtn})
    {
        txt->setFillColor(sf::Color::White);
        txt->setStyle(sf::Text::Bold);
    }

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);

                if (currentPage == Page::MENU)
                {
                    if (boxJouer.getGlobalBounds().contains(mp))
                    {
                        reset_loading();
                        currentPage = Page::LOADING;
                    }
                    else if (boxParam.getGlobalBounds().contains(mp))
                        currentPage = Page::PARAMETRES;
                    else if (boxScenario.getGlobalBounds().contains(mp))
                    {
                        scenario.reset();
                        currentPage = Page::SCENARIO;
                    }
                    else if (boxQuitter.getGlobalBounds().contains(mp))
                        window.close();
                }
                else if (currentPage == Page::PARAMETRES)
                {
                    if (parametres.isBackClicked(mp))
                        currentPage = Page::MENU;
                }
                else if (currentPage == Page::SCENARIO)
                {
                    if (scenario.handleClick(mp))
                        currentPage = Page::MENU;
                }
                else if (currentPage == Page::JEU)
                {
                    sf::Vector2f mp2 = (sf::Vector2f)sf::Mouse::getPosition(window);
                    int result = handleClick_jeu(mp2);
                    if (result == 1) currentPage = Page::MENU;
                    if (result == 2) reset_jeu(font);
                }
            }

            // touches clavier pour le jeu
            if (currentPage == Page::JEU)
            {
                if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::Space || key->code == sf::Keyboard::Key::Up)
                        joueur->sauter();
                    if (key->code == sf::Keyboard::Key::Down)
                        joueur->rouler();
                }
            }

            parametres.handleEvent(*event);
        }

        // Updates
        if (currentPage == Page::SCENARIO)
            scenario.update(dt);

        if (currentPage == Page::LOADING)
        {
            if (update_loading(dt))
            {
                reset_jeu(font);
                currentPage = Page::JEU;
            }
        }

        // 🆕 vérifier en continu si flèche droite maintenue
        if (currentPage == Page::JEU)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                joueur->vitesseX = joueur->vitesseBoost;
            else
                joueur->vitesseX = 0.f;
        }

        if (currentPage == Page::JEU)
            update_jeu(dt, font);

        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);

        auto updateHover = [&](sf::Text& txt, sf::RectangleShape& box) {
            bool hover = box.getGlobalBounds().contains(mp);
            box.setFillColor(hover ? sf::Color(0, 80, 160, 160) : sf::Color(0, 0, 0, 130));
            txt.setFillColor(hover ? sf::Color(180, 220, 255) : sf::Color::White);
            return hover;
        };

        bool h1 = updateHover(jouer, boxJouer);
        bool h2 = updateHover(param, boxParam);
        bool h3 = updateHover(scenarioBtn, boxScenario);
        bool h4 = updateHover(quitterBtn, boxQuitter);

        // Draw
        window.clear();

        if (currentPage == Page::MENU)
        {
            window.draw(background);
            drawStyledButton(window, jouer, boxJouer, h1);
            drawStyledButton(window, param, boxParam, h2);
            drawStyledButton(window, scenarioBtn, boxScenario, h3);
            drawStyledButton(window, quitterBtn, boxQuitter, h4);
        }
        else if (currentPage == Page::PARAMETRES)
            parametres.draw();
        else if (currentPage == Page::SCENARIO)
            scenario.draw();
        else if (currentPage == Page::LOADING)
            draw_loading(window);
        else if (currentPage == Page::JEU)
            draw_jeu(window);

        window.display();
    }

    return 0;
}
