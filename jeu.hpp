#ifndef JEU_HPP
#define JEU_HPP

#include <list>

typedef enum {VIDE, MUR} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;
typedef enum {FUYARD, CHASSEUR, ALEATOIRE, OBSERVATEUR} EtatFantome;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;
    EtatFantome comportement;// permet de définir différents comportement du fantomes 0=direction aléatoire 1=cherche pacman

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
    EtatFantome getComportement() const {return comportement;}
};

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
    Direction dirPacman;
    bool invincible=true;
    bool finJeu=false;
    std::list<Fantome> fantomes;

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;

    void setDirPacman(Direction);

    bool getFin();

    // Retourne la case � une position donn�e
    Case getCase(int, int) const;

    // Retourne la liste de fantomes en lecture seule
    const std::list<Fantome> &getFantomes() const;

    // Indique si la case � une position donn�e existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // D�place Pacman dans une direction (si la case � atteindre est valide)
    bool deplacePacman(Direction);
    //ac�de � la direction du pacman
    Direction getDirPacman() const;

    //ajout et retrait de fantomes
    void RetraitFantome();
    void AjoutFantome();
    //Rrtourne vrai si il y a une colision entre un fantome et pacman, sinon retourne faux 
    bool colisionPacmanFantome();
};

#endif
