#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "Jeu.hpp"

// Background
sf::Texture bg3Texture;
sf::Sprite* bg3Sprite = nullptr;

// Sol
sf::RectangleShape sol;
float SOL_Y       = 520.f;
float PERSO_SOL_Y = 390.f;

// Joueur
Personnage* joueur = nullptr;

// Obstacles
std::vector<Obstacle*> obstacles;

struct InfoObstacle
{
    int   index;
    float x;
    bool  flag;
};

std::vector<InfoObstacle> planObstacles = {
    {1,  1100.f, false},
    {2,  1170.f, false},
    {3,  1550.f, false},
    {4,  1950.f, false},
    {5,  2350.f, false},
    {6,  2750.f, false},
    {7,  2820.f, false},
    {8,  3220.f, false},
    {9,  3620.f, false},
    {10, 4020.f, false},
    {11, 4420.f, false},
    {12, 4490.f, false},
    {13, 4560.f, false},
    {14, 4960.f, false},
    {15, 5030.f, false},
    {16, 5430.f, false},
    {17, 5830.f, false},
    {18, 6230.f, false},
    {19, 6300.f, false},
    {20, 6900.f, true},
};

int   obstacleIndex   = 0;
float offsetX         = 0.f;
float vitesseMonde    = 250.f;

// Vies
int   vies            = 2;
bool  invincible      = false;
float timerInvincible = 0.f;

// Timer
float timerJeu        = 60.f;

// Score
float scoreTimer      = 0.f;
int   score           = 0;

// Etats
bool gameOver            = false;
bool jeuGagne            = false;
bool gameOverEnAttente   = false;
float timerGameOver      = 0.f;
float delaiGameOver      = 0.5f;

// Particules etincelles
struct Particule
{
    float x, y;
    float vitesseX, vitesseY;
    float dureeVie;
    float dureeMax;
    sf::CircleShape forme;
};

std::vector<Particule> particules;

void creerEtincelles(float px, float py)
{
    for (int i = 0; i < 12; i++)
    {
        Particule p;
        p.x = px;
        p.y = py;

        float angle   = (rand() % 360) * 3.14159f / 180.f;
        float vitesse = 120.f + rand() % 200; // 🆕 plus grand
        p.vitesseX    = cos(angle) * vitesse;
        p.vitesseY    = sin(angle) * vitesse;

        p.dureeMax = 0.3f + (rand() % 20) / 100.f;
        p.dureeVie = 0.f;

        p.forme.setRadius(7.f); // 🆕 plus grand
        p.forme.setFillColor(sf::Color(255, 60, 60));
        p.forme.setPosition({px, py});

        particules.push_back(p);
    }
}

// Coeurs
sf::CircleShape coeur1, coeur2;

// Textes
sf::Text* texteScore    = nullptr;
sf::Text* texteTimer    = nullptr;
sf::Text* texteGameOver = nullptr;
sf::Text* texteGagne    = nullptr;
sf::Text* texteQuitter  = nullptr;
sf::Text* texteRepeter  = nullptr;

// Boutons
sf::RectangleShape btnQuitter;
sf::RectangleShape btnRepeter;

void init_jeu(sf::Font& font)
{
    // background
    bg3Texture.loadFromFile("images/background3.png");
    if (bg3Sprite) delete bg3Sprite;
    bg3Sprite = new sf::Sprite(bg3Texture);
    sf::Vector2u s = bg3Texture.getSize();
    bg3Sprite->setScale({1000.f / s.x, 600.f / s.y});

    // sol
    sol.setSize({1000.f, 80.f});
    sol.setPosition({0.f, SOL_Y});
    sol.setFillColor(sf::Color(10, 10, 10));
    sol.setOutlineColor(sf::Color(80, 160, 255));
    sol.setOutlineThickness(2.f);

    // joueur
    if (joueur) delete joueur;
    joueur = new Personnage();
    joueur->solY = PERSO_SOL_Y;
    joueur->y    = PERSO_SOL_Y;

    // obstacles
    for (auto* o : obstacles) delete o;
    obstacles.clear();
    obstacleIndex = 0;
    offsetX       = 0.f;

    // reset variables
    vies                = 2;
    invincible          = false;
    timerInvincible     = 0.f;
    timerJeu            = 60.f;
    score               = 0;
    scoreTimer          = 0.f;
    gameOver            = false;
    jeuGagne            = false;
    gameOverEnAttente   = false;
    timerGameOver       = 0.f;

    // reset particules
    particules.clear();

    // coeurs
    coeur1.setRadius(15.f);
    coeur1.setFillColor(sf::Color(80, 160, 255));
    coeur1.setPosition({20.f, 20.f});

    coeur2.setRadius(15.f);
    coeur2.setFillColor(sf::Color(80, 160, 255));
    coeur2.setPosition({55.f, 20.f});

    // textes
    if (texteScore) delete texteScore;
    texteScore = new sf::Text(font, "Score : 0", 24);
    texteScore->setFillColor(sf::Color::White);
    texteScore->setPosition({400.f, 20.f});

    if (texteTimer) delete texteTimer;
    texteTimer = new sf::Text(font, "60s", 24);
    texteTimer->setFillColor(sf::Color(180, 220, 255));
    texteTimer->setPosition({900.f, 20.f});

    if (texteGameOver) delete texteGameOver;
    texteGameOver = new sf::Text(font, "GAME OVER", 60);
    texteGameOver->setFillColor(sf::Color(80, 180, 255));
    texteGameOver->setStyle(sf::Text::Bold);
    sf::FloatRect gb = texteGameOver->getLocalBounds();
    texteGameOver->setOrigin(gb.position + gb.size / 2.f);
    texteGameOver->setPosition({500.f, 220.f});

    if (texteGagne) delete texteGagne;
    texteGagne = new sf::Text(font, "TU AS GAGNE !", 60);
    texteGagne->setFillColor(sf::Color(80, 180, 255));
    texteGagne->setStyle(sf::Text::Bold);
    sf::FloatRect gg = texteGagne->getLocalBounds();
    texteGagne->setOrigin(gg.position + gg.size / 2.f);
    texteGagne->setPosition({500.f, 220.f});

    btnQuitter.setSize({180.f, 55.f});
    btnQuitter.setFillColor(sf::Color(0, 0, 0, 180));
    btnQuitter.setOutlineColor(sf::Color::White);
    btnQuitter.setOutlineThickness(2.f);
    btnQuitter.setPosition({200.f, 350.f});

    if (texteQuitter) delete texteQuitter;
    texteQuitter = new sf::Text(font, "Quitter", 28);
    texteQuitter->setFillColor(sf::Color::White);
    texteQuitter->setStyle(sf::Text::Bold);
    sf::FloatRect qb = texteQuitter->getLocalBounds();
    texteQuitter->setOrigin(qb.position + qb.size / 2.f);
    texteQuitter->setPosition({290.f, 377.f});

    btnRepeter.setSize({180.f, 55.f});
    btnRepeter.setFillColor(sf::Color(0, 0, 0, 180));
    btnRepeter.setOutlineColor(sf::Color::White);
    btnRepeter.setOutlineThickness(2.f);
    btnRepeter.setPosition({620.f, 350.f});

    if (texteRepeter) delete texteRepeter;
    texteRepeter = new sf::Text(font, "Repeter", 28);
    texteRepeter->setFillColor(sf::Color::White);
    texteRepeter->setStyle(sf::Text::Bold);
    sf::FloatRect rb = texteRepeter->getLocalBounds();
    texteRepeter->setOrigin(rb.position + rb.size / 2.f);
    texteRepeter->setPosition({710.f, 377.f});
}

void reset_jeu(sf::Font& font)
{
    init_jeu(font);
}

int handleClick_jeu(sf::Vector2f mp)
{
    if (gameOver || jeuGagne)
    {
        if (btnQuitter.getGlobalBounds().contains(mp)) return 1;
        if (btnRepeter.getGlobalBounds().contains(mp)) return 2;
    }
    return 0;
}

void update_jeu(float dt, sf::Font& font)
{
    if (gameOver || jeuGagne) return;

    // timer
    timerJeu -= dt;
    if (timerJeu <= 0.f)
    {
        timerJeu = 0.f;
        gameOver = true;
    }
    texteTimer->setString(std::to_string((int)timerJeu) + "s");

    // invincible
    if (invincible)
    {
        timerInvincible += dt;
        if (timerInvincible >= 1.5f)
        {
            invincible      = false;
            timerInvincible = 0.f;
        }
    }

    joueur->update(dt);

    // score
    scoreTimer += dt;
    if (scoreTimer >= 1.f)
    {
        scoreTimer = 0.f;
        score++;
        texteScore->setString("Score : " + std::to_string(score));
    }

    // offset monde
    offsetX += vitesseMonde * dt;

    // spawner obstacles
    while (obstacleIndex < (int)planObstacles.size())
    {
        InfoObstacle& info = planObstacles[obstacleIndex];
        float spawnX = info.x - offsetX + 1000.f;

        if (spawnX <= 1100.f)
        {
            std::string chemin = "images/obstacle" +
                                  std::to_string(info.index) + ".png";
            float posY = SOL_Y - (info.flag ? 100.f : 70.f);
            obstacles.push_back(new Obstacle(spawnX, posY, chemin, info.flag));
            obstacleIndex++;
        }
        else break;
    }

    // update obstacles
    for (auto* o : obstacles)
        o->update(dt);

    // supprimer hors ecran
    for (int i = (int)obstacles.size() - 1; i >= 0; i--)
    {
        if (obstacles[i]->horsEcran())
        {
            if (obstacles[i]->estFlag)
                jeuGagne = true;
            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
        }
    }

    // collision
    if (!invincible && !gameOverEnAttente)
    {
        for (auto* o : obstacles)
        {
            sf::FloatRect r1 = joueur->forme.getGlobalBounds();
            sf::FloatRect r2 = o->forme.getGlobalBounds();
            if (r1.findIntersection(r2))
            {
                if (o->estFlag) { jeuGagne = true; break; }

                // etincelles !
                creerEtincelles(joueur->x + 64.f, joueur->y + 64.f);

                vies--;
                invincible = true;

                if (vies == 1)
                    coeur2.setFillColor(sf::Color(60, 60, 60));

                if (vies <= 0)
                {
                    creerEtincelles(joueur->x + 64.f, joueur->y + 64.f);
                    gameOverEnAttente = true; // attendre etincelles
                }

                break;
            }
        }
    }

    // attendre etincelles avant game over
    if (gameOverEnAttente)
    {
        timerGameOver += dt;
        if (timerGameOver >= delaiGameOver)
            gameOver = true;
    }

    // update particules
    for (int i = (int)particules.size() - 1; i >= 0; i--)
    {
        Particule& p = particules[i];
        p.dureeVie += dt;

        p.x += p.vitesseX * dt;
        p.y += p.vitesseY * dt;

        float ratio = 1.f - p.dureeVie / p.dureeMax;
        p.forme.setRadius(7.f * ratio); // 🆕 plus grand
        int alpha = (int)(255 * ratio);
        p.forme.setFillColor(sf::Color(255, 60, 60, alpha));
        p.forme.setPosition({p.x, p.y});

        if (p.dureeVie >= p.dureeMax)
            particules.erase(particules.begin() + i);
    }
}

void draw_jeu(sf::RenderWindow& window)
{
    if (gameOver)
    {
        sf::RectangleShape fond({1000.f, 600.f});
        fond.setFillColor(sf::Color(5, 15, 45));
        window.draw(fond);
        window.draw(*texteGameOver);

        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);

        bool hQ = btnQuitter.getGlobalBounds().contains(mp);
        btnQuitter.setFillColor(hQ ? sf::Color(0, 80, 160, 200) : sf::Color(0, 0, 0, 180));
        texteQuitter->setFillColor(hQ ? sf::Color(180, 220, 255) : sf::Color::White);
        window.draw(btnQuitter);
        window.draw(*texteQuitter);

        bool hR = btnRepeter.getGlobalBounds().contains(mp);
        btnRepeter.setFillColor(hR ? sf::Color(0, 80, 160, 200) : sf::Color(0, 0, 0, 180));
        texteRepeter->setFillColor(hR ? sf::Color(180, 220, 255) : sf::Color::White);
        window.draw(btnRepeter);
        window.draw(*texteRepeter);
        return;
    }

    if (jeuGagne)
    {
        sf::RectangleShape fond({1000.f, 600.f});
        fond.setFillColor(sf::Color(5, 15, 45));
        window.draw(fond);
        window.draw(*texteGagne);

        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);

        bool hQ = btnQuitter.getGlobalBounds().contains(mp);
        btnQuitter.setFillColor(hQ ? sf::Color(0, 80, 160, 200) : sf::Color(0, 0, 0, 180));
        texteQuitter->setFillColor(hQ ? sf::Color(180, 220, 255) : sf::Color::White);
        window.draw(btnQuitter);
        window.draw(*texteQuitter);

        bool hR = btnRepeter.getGlobalBounds().contains(mp);
        btnRepeter.setFillColor(hR ? sf::Color(0, 80, 160, 200) : sf::Color(0, 0, 0, 180));
        texteRepeter->setFillColor(hR ? sf::Color(180, 220, 255) : sf::Color::White);
        window.draw(btnRepeter);
        window.draw(*texteRepeter);
        return;
    }

    // jeu normal
    window.draw(*bg3Sprite);
    window.draw(sol);

    for (auto* o : obstacles)
        o->draw(window);

    joueur->draw(window);

    // dessiner etincelles
    for (auto& p : particules)
        window.draw(p.forme);

    window.draw(coeur1);
    window.draw(coeur2);
    window.draw(*texteScore);
    window.draw(*texteTimer);
}
