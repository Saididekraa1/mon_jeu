#pragma once
#include <SFML/Graphics.hpp>

// ============================================
// CLASSE MERE
// ============================================
class ObjetJeu
{
public:
    float x, y;
    float largeur, hauteur;
    sf::RectangleShape forme;

    ObjetJeu(float x, float y, float largeur, float hauteur)
    {
        this->x = x;
        this->y = y;
        this->largeur = largeur;
        this->hauteur = hauteur;
        forme.setSize({largeur, hauteur});
        forme.setPosition({x, y});
    }

    virtual void update(float dt) { }

    virtual void draw(sf::RenderWindow& window)
    {
        window.draw(forme);
    }

    virtual ~ObjetJeu() { }
};

// ============================================
// PERSONNAGE
// ============================================
class Personnage : public ObjetJeu
{
public:
    float vitesseY = 0.f;
    float gravite = 1500.f;
    float forceSaut = -600.f;
    bool enAir = false;
    float solY = 390.f;

    // 🆕 vitesse horizontale
    float vitesseX       = 0.f;
    float vitesseBoost   = 150.f;
    float xDepart        = 100.f;  // position de depart

    // textures
    sf::Texture texRun, texJump, texDoubleJump, texRoll;
    sf::Sprite* sprite = nullptr;

    // animation
    int frameActuelle = 0;
    float timerAnim = 0.f;
    float vitesseAnim = 0.08f;

    // double jump
    int nbSauts = 0;
    int maxSauts = 2;

    enum Etat { COURSE, SAUT, DOUBLE_SAUT, ROULADE };
    Etat etat = COURSE;
    int nbFrames = 12;

    Personnage() : ObjetJeu(100, 390, 128, 128)
    {
        texRun.loadFromFile("images/Running.png");
        texJump.loadFromFile("images/Jumping.PNG");
        texDoubleJump.loadFromFile("images/Double_Jump.PNG");
        texRoll.loadFromFile("images/Roll.png");

        sprite = new sf::Sprite(texRun);
        sprite->setTextureRect(sf::IntRect({0, 0}, {128, 128}));
        sprite->setPosition({x, y});
        sprite->setScale({1.0f, 1.0f});

        forme.setFillColor(sf::Color::Transparent);
    }

    ~Personnage()
    {
        delete sprite;
    }

    void sauter()
    {
        if (nbSauts < maxSauts && etat != ROULADE)
        {
            vitesseY = forceSaut;
            enAir = true;

            if (nbSauts == 0)
                changerEtat(SAUT);
            else
                changerEtat(DOUBLE_SAUT);

            nbSauts++;
        }
    }

    void rouler()
    {
        if (!enAir)
            changerEtat(ROULADE);
    }

    void changerEtat(Etat nouvelEtat)
    {
        if (etat == nouvelEtat) return;

        etat = nouvelEtat;
        frameActuelle = 0;
        timerAnim = 0.f;

        delete sprite;

        if (etat == COURSE)       { sprite = new sf::Sprite(texRun);        nbFrames = 12; }
        if (etat == SAUT)         { sprite = new sf::Sprite(texJump);       nbFrames = 10; }
        if (etat == DOUBLE_SAUT)  { sprite = new sf::Sprite(texDoubleJump); nbFrames = 11; }
        if (etat == ROULADE)      { sprite = new sf::Sprite(texRoll);       nbFrames = 9;  }

        sprite->setTextureRect(sf::IntRect({0, 0}, {128, 128}));
        sprite->setScale({1.0f, 1.0f});
    }

    void update(float dt) override
    {
        // 🆕 mouvement horizontal
        x += vitesseX * dt;
        if (x > 300.f) x = 300.f;   // limite droite
        if (x < xDepart) x = xDepart; // limite gauche (retour position normale)

        // gravity
        vitesseY += gravite * dt;
        y += vitesseY * dt;

        // landing
        if (y >= solY)
        {
            y = solY;
            vitesseY = 0.f;
            enAir = false;
            nbSauts = 0;

            if (etat == SAUT || etat == DOUBLE_SAUT)
                changerEtat(COURSE);
        }

        // animation
        timerAnim += dt;
        if (timerAnim >= vitesseAnim)
        {
            timerAnim = 0.f;
            frameActuelle++;

            if (frameActuelle >= nbFrames)
            {
                frameActuelle = 0;

                if (etat == ROULADE)
                    changerEtat(COURSE);
            }
        }

        sprite->setTextureRect(sf::IntRect({frameActuelle * 128, 0}, {128, 128}));
        sprite->setPosition({x, y});

        forme.setPosition({x + 20.f, y + 10.f});
        forme.setSize({88.f, 108.f});
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(*sprite);
    }
};

// ============================================
// OBSTACLE
// ============================================
class Obstacle : public ObjetJeu
{
public:
    float vitesse = 250.f;
    sf::Texture texture;
    sf::Sprite* sprite = nullptr;
    bool estFlag = false;

    Obstacle(float x, float y, std::string cheminImage, bool flag = false)
        : ObjetJeu(x, y, flag ? 80.f : 70.f, flag ? 100.f : 70.f)
    {
        estFlag = flag;
        texture.loadFromFile(cheminImage);

        sprite = new sf::Sprite(texture);

        sf::Vector2u s = texture.getSize();
        float scaleX = largeur / s.x;
        float scaleY = hauteur / s.y;
        sprite->setScale({scaleX, scaleY});
        sprite->setPosition({x, y});

        forme.setFillColor(sf::Color::Transparent);
        forme.setSize({largeur - 10.f, hauteur - 5.f});
    }

    ~Obstacle()
    {
        delete sprite;
    }

    void update(float dt) override
    {
        x -= vitesse * dt;
        sprite->setPosition({x, y});
        forme.setPosition({x + 5.f, y + 5.f});
    }

    bool horsEcran()
    {
        return x + largeur < 0;
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(*sprite);
    }
};
