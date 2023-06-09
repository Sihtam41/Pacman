#ifndef JEU_HPP
#define JEU_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include <list>
#include "AStar.hpp"

typedef enum {VIDE, MUR} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;
typedef enum {FUYARD, ALEATOIRE, OBSERVATEUR, TRAQUEUR} EtatFantome;

class Jeu;
//structure qui définit une coordonnée
typedef struct Coord {
    int x;
    int y;
}Coord;
//structure qui définit un portail
typedef struct Portail {
    Coord Entree;
    Coord Sortie;
}Portail;
//Classe qui définit un fantome
class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;
    EtatFantome comportement;// permet de définir différents comportement du fantomes 
    EtatFantome comportementActuel;// permet de définir le comportement actuel du fantome

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
    int vitFantome=2;// la vitesse maximale est de 10
    EtatFantome getComportement() const {return comportement;}
    EtatFantome getComportementActuel() const {return comportementActuel;}
};

class PacBalls
{
    friend class Jeu;

    protected:
        int posX, posY;

    public:
        PacBalls();
        int getPosX() const;
        int getPosY() const;
};

class Balls
{
    friend class Jeu;

    protected:
        int posX, posY;

    public:
        Balls();
        int getPosX() const;
        int getPosY() const;
};

class Jeu : public QFrame
{
  protected:
    Case *terrain;
    AStar::Generator generator;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
    int vitPacman=4;// la vitesse maximale est de 10
    Direction dirPacman;
    bool invincible=false;
    QTimer *timerInvincible;//timer qui gère les collisions
    bool finJeu=false;
    std::list<Fantome> fantomes;
    std::list<PacBalls> list_pacballs;
    std::list<Balls> list_balls;
    int score=0; 
    uint32_t Frame=0;//définit le nombre de Frame écoulé depuis le début du cycle (sert à définir la vitesse)

    std::list<Portail> portail;//liste des portails

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init(int);
    void reset();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;

    void setDirPacman(Direction);

    bool getFin();
    bool NiveauFini();

    // Retourne la case � une position donn�e
    Case getCase(int, int) const;

    // Retourne la liste de fantomes en lecture seule
    const std::list<Fantome> &getFantomes() const;
    //liste des pacballs
    std::list<PacBalls> &getPacBalls();
    //liste des balls
    std::list<Balls> &getBalls();
    //Distance entre deux points
    float Distance(int, int, int, int);
    // Indique si la case � une position donn�e existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;
    // Seul pacman peut passer par les portails
    bool posPortail(int, int);
    Coord Teleportation(int, int);// renvois l'index du portail si la position est un portail, sinon renvois -1

    // D�place Pacman dans une direction (si la case � atteindre est valide)
    bool deplacePacman(Direction);
    //ac�de � la direction du pacman
    Direction getDirPacman() const;

    //ajout et retrait de fantomes
    void RetraitFantome();
    void AjoutFantome();
    //Rrtourne vrai si il y a une colision entre un fantome et pacman, sinon retourne faux 
    bool colisionPacmanFantome();
    
    //Test la collision entre le pacman et les pacballs
    void collisionPacballs();
    //Test la collision entre le pacman et les balls
    void collisionBalls();
    void ArretInvincibilite();
    bool getInvincibilite() const;

    //Gestion du score
    int getScore() const;    
};

#endif
