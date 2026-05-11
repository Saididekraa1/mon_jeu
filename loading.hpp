#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

// Variables
static sf::RectangleShape fond_loading;
static sf::RectangleShape barre_bg;
static sf::RectangleShape barre_fill;
static std::optional<sf::Text> texte_loading; // ✅ FIX SFML 3

static float loading_timer = 0.f;
static float loading_duree = 3.f;

// INIT
void init_loading(sf::Font& font)
{
    fond_loading.setSize({1000.f, 600.f});
    fond_loading.setFillColor(sf::Color(5, 15, 45));

    barre_bg.setSize({600.f, 20.f});
    barre_bg.setPosition({200.f, 320.f});
    barre_bg.setFillColor(sf::Color(20, 50, 100));
    barre_bg.setOutlineColor(sf::Color(60, 140, 220));
    barre_bg.setOutlineThickness(1.f);

    barre_fill.setSize({0.f, 20.f});
    barre_fill.setPosition({200.f, 320.f});
    barre_fill.setFillColor(sf::Color(80, 180, 255));

    // ✅ SFML 3 TEXT INIT
    texte_loading = sf::Text(font, "LOADING...", 22);
    texte_loading->setFillColor(sf::Color(150, 210, 255));
    texte_loading->setPosition({430.f, 290.f});

    loading_timer = 0.f;
}

// UPDATE
bool update_loading(float dt)
{
    loading_timer += dt;

    float progress = loading_timer / loading_duree;
    if (progress > 1.f) progress = 1.f;

    barre_fill.setSize({600.f * progress, 20.f});

    return loading_timer >= loading_duree;
}

// DRAW
void draw_loading(sf::RenderWindow& window)
{
    window.draw(fond_loading);
    window.draw(barre_bg);
    window.draw(barre_fill);

    if (texte_loading) // safety
        window.draw(*texte_loading);
}

// RESET
void reset_loading()
{
    loading_timer = 0.f;
    barre_fill.setSize({0.f, 20.f});
}
