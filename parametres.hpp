#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <algorithm>

class Parametres {
public:
    sf::RenderWindow& window;
    sf::Font& font;
    sf::Music& music;

    float volumeSon = 100.f;
    float volumeMusique = 100.f;
    bool draggingSon = false;
    bool draggingMus = false;

    float sliderX = 250.f, sliderW = 500.f;
    float sliderY_son = 310.f, sliderY_mus = 400.f;

    // Onglet actif
    enum Onglet { JEU, COMMANDES };
    Onglet ongletActif = JEU;

    // Header
    sf::RectangleShape headerBg, headerLine;
    sf::Text titleText, backText;

    // Onglets
    sf::Text tabJeu, tabCommandes;
    sf::RectangleShape tabUnderlineJeu, tabUnderlineCommandes;

    // Langue
    sf::Text langLabel, langValue, langLeft, langRight;

    // Slider SON
    sf::RectangleShape sliderSonBg, sliderSonFill, sliderSonHandle;
    sf::Text labelSon, valueSonText;

    // Slider MUSIQUE
    sf::RectangleShape sliderMusBg, sliderMusFill, sliderMusHandle;
    sf::Text labelMus, valueMus;

    // Commandes
    sf::Text cmdTitre;
    sf::Text cmdDesc;
    sf::Text cmdSauter, cmdRouler, cmdAccelerer;

    Parametres(sf::RenderWindow& win, sf::Font& f, sf::Music& m)
        : window(win), font(f), music(m),
          titleText(f, "PARAMETRES", 20),
          backText(f, "<   PARAMETRES", 20),
          tabJeu(f, "JEU", 18),
          tabCommandes(f, "COMMANDES", 18),
          langLabel(f, "LANGUE", 14),
          langValue(f, "Francais", 30),
          langLeft(f, "<", 30),
          langRight(f, ">", 30),
          labelSon(f, "SON", 14),
          valueSonText(f, "100%", 15),
          labelMus(f, "MUSIQUE", 14),
          valueMus(f, "100%", 15),
          cmdTitre(f, "GUIDE DU JEU", 22),
          cmdDesc(f, "Evite les obstacles et atteins le drapeau pour gagner !\nUtilise les touches ci-dessous pour controler ton personnage :", 16),
          cmdSauter(f, "Sauter        :   Fleche Haut  /  Espace", 22),
          cmdRouler(f, "Rouler        :   Fleche Bas", 22),
          cmdAccelerer(f, "Accelerer  :   Fleche Droite", 22)
    {
        setup();
    }

    void setup()
    {
        // Header
        headerBg.setSize({1000.f, 65.f});
        headerBg.setFillColor(sf::Color(8, 35, 75, 230));
        headerBg.setPosition({0.f, 0.f});

        headerLine.setSize({1000.f, 2.f});
        headerLine.setFillColor(sf::Color(60, 140, 220, 180));
        headerLine.setPosition({0.f, 65.f});

        backText.setFillColor(sf::Color(180, 220, 255));
        backText.setStyle(sf::Text::Bold);
        backText.setPosition({30.f, 20.f});

        // Onglet JEU
        tabJeu.setFillColor(sf::Color::White);
        tabJeu.setStyle(sf::Text::Bold);
        tabJeu.setPosition({380.f, 80.f});

        tabUnderlineJeu.setSize({45.f, 2.f});
        tabUnderlineJeu.setFillColor(sf::Color::White);
        tabUnderlineJeu.setPosition({380.f, 108.f});

        // Onglet COMMANDES
        tabCommandes.setFillColor(sf::Color(150, 190, 230));
        tabCommandes.setStyle(sf::Text::Bold);
        tabCommandes.setPosition({470.f, 80.f});

        tabUnderlineCommandes.setSize({115.f, 2.f});
        tabUnderlineCommandes.setFillColor(sf::Color(150, 190, 230));
        tabUnderlineCommandes.setPosition({470.f, 108.f});

        // LANGUE
        langLabel.setFillColor(sf::Color(150, 190, 230));
        langLabel.setLetterSpacing(2.f);
        centerX(langLabel, 175.f);

        langValue.setFillColor(sf::Color::White);
        langValue.setStyle(sf::Text::Bold | sf::Text::Italic);
        centerX(langValue, 205.f);

        langLeft.setFillColor(sf::Color(150, 190, 230));
        langLeft.setPosition({330.f, 200.f});

        langRight.setFillColor(sf::Color(150, 190, 230));
        langRight.setPosition({660.f, 200.f});

        // Slider SON
        labelSon.setFillColor(sf::Color(150, 190, 230));
        labelSon.setLetterSpacing(2.f);
        centerX(labelSon, sliderY_son - 30.f);

        setupSlider(sliderSonBg, sliderSonFill, sliderSonHandle, sliderY_son, volumeSon);

        valueSonText.setFillColor(sf::Color::White);
        valueSonText.setPosition({sliderX + sliderW + 20.f, sliderY_son - 10.f});

        labelMus.setFillColor(sf::Color(150, 190, 230));
        labelMus.setLetterSpacing(2.f);
        centerX(labelMus, sliderY_mus - 30.f);

        setupSlider(sliderMusBg, sliderMusFill, sliderMusHandle, sliderY_mus, volumeMusique);

        valueMus.setFillColor(sf::Color::White);
        valueMus.setPosition({sliderX + sliderW + 20.f, sliderY_mus - 10.f});

        // Commandes - Titre
        cmdTitre.setFillColor(sf::Color(180, 220, 255));
        cmdTitre.setStyle(sf::Text::Bold);
        centerX(cmdTitre, 130.f);

        // Commandes - Description
        cmdDesc.setFillColor(sf::Color(150, 190, 230));
        centerX(cmdDesc, 175.f);

        // Commandes - Touches
        cmdSauter.setFillColor(sf::Color::White);
        cmdSauter.setStyle(sf::Text::Bold);
        centerX(cmdSauter, 290.f);

        cmdRouler.setFillColor(sf::Color::White);
        cmdRouler.setStyle(sf::Text::Bold);
        centerX(cmdRouler, 370.f);

        cmdAccelerer.setFillColor(sf::Color::White);
        cmdAccelerer.setStyle(sf::Text::Bold);
        centerX(cmdAccelerer, 450.f);
    }

    void centerX(sf::Text& txt, float y)
    {
        sf::FloatRect b = txt.getLocalBounds();
        txt.setOrigin({b.position.x + b.size.x / 2.f, 0.f});
        txt.setPosition({500.f, y});
    }

    void setupSlider(sf::RectangleShape& bg, sf::RectangleShape& fill,
                     sf::RectangleShape& handle, float y, float vol)
    {
        bg.setSize({sliderW, 5.f});
        bg.setPosition({sliderX, y});
        bg.setFillColor(sf::Color(20, 60, 120));

        float fw = (vol / 100.f) * sliderW;
        fill.setSize({fw, 5.f});
        fill.setPosition({sliderX, y});
        fill.setFillColor(sf::Color(80, 160, 255));

        handle.setSize({16.f, 28.f});
        handle.setFillColor(sf::Color(80, 160, 255));
        handle.setPosition({sliderX + fw - 8.f, y - 12.f});
    }

    void updateSlider(sf::RectangleShape& fill, sf::RectangleShape& handle,
                      float y, float& vol, float mx, sf::Text& valText)
    {
        vol = std::clamp((mx - sliderX) / sliderW * 100.f, 0.f, 100.f);
        float fw = (vol / 100.f) * sliderW;
        fill.setSize({fw, 5.f});
        handle.setPosition({sliderX + fw - 8.f, y - 12.f});
        valText.setString(std::to_string((int)vol) + "%");
    }

    void handleEvent(const sf::Event& event)
    {
        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (event.is<sf::Event::MouseButtonPressed>())
        {
            // Clic sur onglets
            if (tabJeu.getGlobalBounds().contains(mp))
            {
                ongletActif = JEU;
                tabJeu.setFillColor(sf::Color::White);
                tabUnderlineJeu.setFillColor(sf::Color::White);
                tabCommandes.setFillColor(sf::Color(150, 190, 230));
                tabUnderlineCommandes.setFillColor(sf::Color(150, 190, 230));
            }
            if (tabCommandes.getGlobalBounds().contains(mp))
            {
                ongletActif = COMMANDES;
                tabCommandes.setFillColor(sf::Color::White);
                tabUnderlineCommandes.setFillColor(sf::Color::White);
                tabJeu.setFillColor(sf::Color(150, 190, 230));
                tabUnderlineJeu.setFillColor(sf::Color(150, 190, 230));
            }

            if (ongletActif == JEU)
            {
                if (sliderSonHandle.getGlobalBounds().contains(mp))  draggingSon = true;
                if (sliderMusHandle.getGlobalBounds().contains(mp))  draggingMus = true;
            }
        }
        if (event.is<sf::Event::MouseButtonReleased>()) {
            draggingSon = false;
            draggingMus = false;
        }
        if (event.is<sf::Event::MouseMoved>()) {
            if (draggingSon) {
                updateSlider(sliderSonFill, sliderSonHandle, sliderY_son, volumeSon, mp.x, valueSonText);
                music.setVolume(volumeSon);
            }
            if (draggingMus) {
                updateSlider(sliderMusFill, sliderMusHandle, sliderY_mus, volumeMusique, mp.x, valueMus);
                music.setVolume(volumeMusique);
            }
        }
    }

    bool isBackClicked(sf::Vector2f mp) {
        return backText.getGlobalBounds().contains(mp);
    }

    void draw()
    {
        sf::RectangleShape bg({1000.f, 600.f});
        bg.setFillColor(sf::Color(5, 20, 55, 255));
        window.draw(bg);

        for (int i = 0; i < 8; i++) {
            sf::RectangleShape line({800.f, 1.f});
            line.setFillColor(sf::Color(255, 255, 255, 8));
            line.setRotation(sf::degrees(-20.f));
            line.setPosition({(float)(i * 150 - 100), 0.f});
            window.draw(line);
        }

        window.draw(headerBg);
        window.draw(headerLine);
        window.draw(backText);

        // Toujours afficher les deux onglets
        window.draw(tabJeu);
        window.draw(tabUnderlineJeu);
        window.draw(tabCommandes);
        window.draw(tabUnderlineCommandes);

        // Contenu selon onglet actif
        if (ongletActif == JEU)
        {
            window.draw(langLabel);
            window.draw(langLeft);
            window.draw(langValue);
            window.draw(langRight);
            window.draw(labelSon);
            window.draw(sliderSonBg);
            window.draw(sliderSonFill);
            window.draw(sliderSonHandle);
            window.draw(valueSonText);
            window.draw(labelMus);
            window.draw(sliderMusBg);
            window.draw(sliderMusFill);
            window.draw(sliderMusHandle);
            window.draw(valueMus);
        }
        else if (ongletActif == COMMANDES)
        {
            window.draw(cmdTitre);
            window.draw(cmdDesc);
            window.draw(cmdSauter);
            window.draw(cmdRouler);
            window.draw(cmdAccelerer);
        }
    }
};
