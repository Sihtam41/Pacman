#include <iostream>
#include <cassert>
#include <algorithm>
#include <math.h>
#include "jeu.hpp"

using namespace std;

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
    int choix = rand()%10;
    if (choix <= 0)//10% de chance d'être fuyard
        comportement = TRAQUEUR;
    else if (choix <= 4)//40% de chance d'être chasseur
        comportement = OBSERVATEUR;
    else//50% de chance d'être ALÉATOIRE
        comportement = ALEATOIRE;

}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}


Jeu::Jeu()
{
    terrain = nullptr;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
    dirPacman = DROITE;
}

Jeu::Jeu(const Jeu &jeu):fantomes(jeu.fantomes)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    posPacmanX = jeu.posPacmanX;
    posPacmanY = jeu.posPacmanY;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
}

Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain!=nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    posPacmanX = jeu.posPacmanX;
    posPacmanY = jeu.posPacmanY;
    fantomes = jeu.fantomes;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init(int levelNum)
{
	int x, y;
	list<Fantome>::iterator itFantome;

	//Level 1

	if(levelNum == 0)
    {
    const char terrain_defaut[15][22] = {
            "#####################",
            "..........#.........#",
            "#.###.###.#.###.###.#",
            "#...................#",
            "#.##.##.#####.##.##.#",
            "#.##.##.#####.##.##.#",
            "#.......#####.......#",
            "#######.......#######",
            "#.......#####.......#",
            "#.##.##.#...#.##.##.#",
            "#.##.##.#.#.#.##.##.#",
            "#...................#",
            "#.#####.#####.#######",
            "#....................",
            "#####################"
        };

        //on définit l'entrée et la sortie des deux portails


        portail.resize(2);//définit le nombre de fantomes au lacement du niveau
        portail.push_back({-1,1,20,13});
        portail.push_back({21,13,0,1});


	largeur = 21;
	hauteur = 15;

	terrain = new Case[largeur*hauteur];

    generator.setWorldSize({largeur, hauteur});// met en place la taille du terrain pour l'algo Astar

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
            {
                terrain[y*largeur+x] = MUR;
                generator.addCollision({x, y});//ajoute les murs au terrain pour l'algo Astar
            }
            else
                terrain[y*largeur+x] = VIDE;

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    posPacmanX = x,
    posPacmanY = y;

    }

    ////Level 2

    if(levelNum == 1)
{
    const char terrain_default[15][22] = {
            "#####################",
            "#....................",
            "#.#...#...#.#...#...#",
            "#.#.#.#.#.#.#.#.#.#.#",
            "#...................#",
            "#.#.#.#.#.#.#.#.#.#.#",
            "#.#.#.#.#.#.#.#.#.#.#",
            "#...................#",
            "#.#.#.#.#.#.#.#...#.#",
            "#.#.#.#.#.#.#.#...#.#",
            "#...................#",
            "#.#...#.#.#.#.#.#.#.#",
            "#.#...#.#.#.#.#.#.#.#",
            "....................#",
            "#####################"
        };

        portail.resize(2);
        portail.push_back({21,1,0,13});
        portail.push_back({-1,13,20,1});

        largeur = 21;
        hauteur = 15;

        terrain = new Case[largeur*hauteur];

        generator.setWorldSize({largeur, hauteur});

        for(y=0;y<hauteur;++y)
            for(x=0;x<largeur;++x)
                if (terrain_default[y][x]=='#')
                {
                    terrain[y*largeur+x] = MUR;
                    generator.addCollision({x, y});
                }
                else
                    terrain[y*largeur+x] = VIDE;

        posPacmanX = 10,
        posPacmanY = 7;
}

    //Level 3
    if(levelNum == 2)
{
    const char terrain_default[15][22] = {
            "#####################",
            "#.........#..........",
            "#.##.#.#######.#.##.#",
            "#....#....#....#....#",
            "###..####.#.####..###",
            "#..#....#.#.#....#..#",
            "###..####.#.####..###",
            "#....#.........#....#",
            "#.##.#.#######.#.##.#",
            "#..#.............#..#",
            "##..##.#########.#.##",
            "#.........#.........#",
            "#.#######.#.#######.#",
            "....................#",
            "#####################"
        };

        portail.resize(2);
        portail.push_back({-1,1,20,13});
        portail.push_back({21,13,0,1});

        largeur = 22;
        hauteur = 15;

        terrain = new Case[largeur*hauteur];

        generator.setWorldSize({largeur, hauteur});

        for(y=0;y<hauteur;++y)
            for(x=0;x<largeur;++x)
                if (terrain_default[y][x]=='#')
                {
                    terrain[y*largeur+x] = MUR;
                    generator.addCollision({x, y});
                }
                else
                    terrain[y*largeur+x] = VIDE;

        posPacmanX = 10,
        posPacmanY = 7;
}
    //Level 4
    if(levelNum == 3)
    {
    const char terrain_default[15][22] = {
            "#####################",
            "..........#.........#",
            "#.###.###.#.###.###.#",
            "#...................#",
            "#.##.##.#####.##.##.#",
            "#.##.##.#...#.##.##.#",
            "#.......##-##.......#",
            "#######.......#######",
            "#.......#####.......#",
            "#.##.##.#...#.##.##.#",
            "#.##.##.#.#.#.##.##.#",
            "#...................#",
            "#.#####.#####.#######",
            "#....................",
            "#####################"
};


      portail.resize(2);
        portail.push_back({21,1,0,13});
        portail.push_back({-1,13,20,1});

        largeur = 21;
        hauteur = 15;

        terrain = new Case[largeur*hauteur];

        generator.setWorldSize({largeur, hauteur});

        for(y=0;y<hauteur;++y)
            for(x=0;x<largeur;++x)
                if (terrain_default[y][x]=='#')
                {
                    terrain[y*largeur+x] = MUR;
                    generator.addCollision({x, y});
                }
                else
                    terrain[y*largeur+x] = VIDE;

        posPacmanX = 10,
        posPacmanY = 7;

}

    //////////////

    fantomes.resize(4);//définit le nombre de fantomes au lacement du niveau

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);// on vérifie que le fantome n'apparaisse pas dans un mur

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }


    return true;
}

void Jeu::evolue()
{
    //////////////Gestion Frame////////////////////////
    Frame++;

    ////////////////////////////////Deplacement des fantomes///////////////////////////////
    int testX, testY;
	list<Fantome>::iterator itFantome;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    // boucle qui décrit le deplacement futur de fantomes
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if(Frame%(10-itFantome->vitFantome)==0)
        {
            bool deplace = false;
            list<Direction> Dir_test;
            while (!deplace)
            {
                    if (itFantome->comportement==ALEATOIRE)// le fantomes va changer de direction de façon aléatoire si il rencontre un mur
                    {
                        int testX2 = itFantome->posX + depX[itFantome->dir];
                        int testY2 = itFantome->posY + depY[itFantome->dir];
                        if (terrain[testY2*largeur+testX2]!=VIDE)//si le fantome ne va pas sur un mur
                            itFantome->dir = (Direction)(rand()%4);
                    }

                    else if (itFantome->comportement==FUYARD)//Fonctionne mais pas forcément super
                    //Indiquer la direction que le fantome ne peut pas prendre et non choisir la direction dans laquelle il doit aller
                    {
                        int x=itFantome->posX;
                        int y=itFantome->posY;
                        int Px=posPacmanX;
                        int Py=posPacmanY;

                        int fX,fY;//Poisition futur
                        float  dMax=-1;
                        int xMax,yMax;

                        //on test la postiion au dessus du fantomes
                        fX=x+1;
                        fY=y;
                        if(Distance(x,y,Px,Py)<= 5)
                        {
                            if (posValide(fX,fY))
                            {
                                float d=Distance(fX,fY,Px,Py);
                                if(d>=dMax)
                                    {
                                        dMax=d;
                                        itFantome->dir=DROITE;
                                    }
                            }
                            fX=x-1;
                            fY=y;
                            if (posValide(fX,fY))
                            {
                                float d=Distance(fX,fY,Px,Py);
                                if(d>=dMax)
                                    {
                                        dMax=d;
                                        itFantome->dir=GAUCHE;
                                    }
                            }
                            fX=x;
                            fY=y+1;
                            if (posValide(fX,fY))
                            {
                                float d=Distance(fX,fY,Px,Py);
                                if(d>=dMax)
                                    {
                                        dMax=d;
                                        itFantome->dir=BAS;
                                    }
                            }
                            fX=x;
                            fY=y-1;
                            if (posValide(fX,fY))
                            {
                                float d=Distance(fX,fY,Px,Py);
                                if(d>=dMax)
                                    {
                                        dMax=d;
                                        itFantome->dir=HAUT;
                                    }
                            }
                        }
                        else
                        {
                            int testX2 = itFantome->posX + depX[itFantome->dir];
                            int testY2 = itFantome->posY + depY[itFantome->dir];
                            if (terrain[testY2*largeur+testX2]!=VIDE)//si le fantome ne va pas sur un mur
                            itFantome->dir = (Direction)(rand()%4);
                           }







                    }
                    else if (itFantome->comportement==CHASSEUR)// Ne fonctione pas super bien
                    {

                        auto it_DROITE = find(Dir_test.begin(), Dir_test.end(), DROITE);
                        auto it_GAUCHE = find(Dir_test.begin(), Dir_test.end(), GAUCHE);
                        auto it_HAUT = find(Dir_test.begin(), Dir_test.end(), HAUT);
                        auto it_BAS = find(Dir_test.begin(), Dir_test.end(), BAS);
                        // Déplacement vers le pacman
                        if (itFantome->posX<posPacmanX && it_DROITE == Dir_test.end())
                            itFantome->dir = DROITE;
                        else if (itFantome->posX>posPacmanX && it_GAUCHE == Dir_test.end())
                            itFantome->dir = GAUCHE;
                        else if (itFantome->posY<posPacmanY && it_BAS == Dir_test.end())
                            itFantome->dir = BAS;
                        else if (itFantome->posY>posPacmanY && it_HAUT == Dir_test.end())
                            itFantome->dir = HAUT;
                        else
                            itFantome->dir = (Direction)(rand()%4);


                        Dir_test.push_back(itFantome->dir);
                    }
                    else if (itFantome->comportement==TRAQUEUR)//Fonctionnel
                    {
                        auto path = generator.findPath({itFantome->posX,itFantome->posY}, {posPacmanX, posPacmanY});// créer le chemin
                        if (path.size()>1)// qaund il a trouvé un chemin supérieur à 1
                        {
                            int curseurProchainDeplacement= path.size()-2;// cherche l'avant dernier case qui correspond au prochain déplacement
                            if (path[curseurProchainDeplacement].x>itFantome->posX)// si il doit aller à droite
                                itFantome->dir = DROITE;
                            else if (path[curseurProchainDeplacement].x<itFantome->posX)
                                itFantome->dir = GAUCHE;
                            else if (path[curseurProchainDeplacement].y>itFantome->posY)
                                itFantome->dir = BAS;
                            else if (path[curseurProchainDeplacement].y<itFantome->posY)
                                itFantome->dir = HAUT;
                        }
                        else// Si il ne trouve pas de chemin
                            itFantome->dir = (Direction)(rand()%4);


                    }
                    else if (itFantome->comportement==OBSERVATEUR)//Fonctionnel
                    {
                        int x=itFantome->posX;
                        int y=itFantome->posY;
                        int Px=posPacmanX;
                        int Py=posPacmanY;

                        bool PACMAN_VU = false;//Variable qui définit si la pacman a été vu ou pas

                        Direction dirFantomeCourante = itFantome->dir;
                        //on regarde si le pacman est dans la meme ligne ou colonne que le fantome
                        if (y==Py )// si le fantome est sur la même ligne que  le Pacman
                        {
                            PACMAN_VU = true;
                            if (x<=Px )// si le fantome est à droite
                            {
                                itFantome->dir = DROITE;
                                for (int i=x; i<Px; i++)// on regarde si il y a un mur entre le fantome est pacman
                                {
                                    if (getCase(i, y)==MUR)
                                    {
                                        itFantome->dir = dirFantomeCourante;
                                        PACMAN_VU = false;//il y a un mur donc le fantome n'a pas vu le pacman
                                        break;
                                    }
                                }
                            }
                            else if (x>=Px )// si le fantome est à gauche
                            {
                                itFantome->dir = GAUCHE;
                                for (int i=x; i>Px; i--)// on regarde si il y a un mur entre le fantome est pacman
                                {
                                    if (getCase(i, y)==MUR)
                                    {
                                        itFantome->dir = dirFantomeCourante;
                                        PACMAN_VU = false;//il y a un mur donc le fantome n'a pas vu le pacman
                                        break;
                                    }
                                }
                            }
                        }
                        else if (x==Px )// si le fantome est sur la même colonne que  le Pacman
                        {
                            PACMAN_VU = true;
                            if (y<=Py )// si le fantome est en bas
                            {
                                itFantome->dir = BAS;
                                for (int i=y; i<Py; i++)// on regarde si il y a un mur entre le fantome est pacman
                                {
                                    if (getCase(x, i)==MUR)
                                    {
                                        itFantome->dir = dirFantomeCourante;
                                        PACMAN_VU = false;//il y a un mur donc le fantome n'a pas vu le pacman
                                        break;
                                    }
                                }
                            }
                            else if (y>=Py )// si le fantome est en haut
                            {
                                itFantome->dir = HAUT;
                                for (int i=y; i>Py; i--)// on regarde si il y a un mur entre le fantome est pacman
                                {
                                    if (getCase(x, i)==MUR)
                                    {
                                        itFantome->dir = dirFantomeCourante;
                                        PACMAN_VU = false;//il y a un mur donc le fantome n'a pas vu le pacman
                                        break;
                                    }
                                }
                            }
                        }
                        else
                            PACMAN_VU = false;//le fantome n'a pas vu le pacman

                        if (PACMAN_VU==false)//Comme le comportenement ALEATOIRE
                        {
                            int testX2 = itFantome->posX + depX[itFantome->dir];
                            int testY2 = itFantome->posY + depY[itFantome->dir];
                            if (terrain[testY2*largeur+testX2]!=VIDE)//si le fantome ne va pas sur un mur
                                itFantome->dir = (Direction)(rand()%4);
                        }

                    }


                testX = itFantome->posX + depX[itFantome->dir];
                testY = itFantome->posY + depY[itFantome->dir];

                if (posValide(testX,testY))//si le fantome ne va pas sur un mur
                {//alors il se deplace
                    itFantome->posX = testX;
                    itFantome->posY = testY;

                    deplace=true;
                }
            }
        }


    }
//////////////////////////////////////////////////////////////////////////////////////////

    deplacePacman(dirPacman);

    finJeu=colisionPacmanFantome();
}
//Note: les colision ne sont pas parfaite car si le pacman et un fantome se colle et ensuite qu'ils vont tout les deux l'un vers l'autre, alors il n'y a pas de colision
//id�e : voirt les position pr�c�dentes du pacman ?
bool Jeu::colisionPacmanFantome()
{

	list<Fantome>::iterator itFantome;


    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if ((itFantome->posX == posPacmanX) && (itFantome->posY == posPacmanY))
        {
            if (invincible==false)
                return true;
            else
                fantomes.erase(itFantome);
        }
    }

    return false;
}
float Jeu::Distance(int a, int b, int c, int d)
{
    return sqrt((a-c)*(a-c) + (b-d)*(b-d));
}
int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getPacmanX() const
{
    return posPacmanX;
}

int Jeu::getPacmanY() const
{
    return posPacmanY;
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

Direction Jeu::getDirPacman() const
{
    return dirPacman;
}

const std::list<Fantome> &Jeu::getFantomes() const
{
    return fantomes;
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]==VIDE);
}
void Jeu::setDirPacman(Direction dir)
{
    dirPacman=dir;
}

bool Jeu::getFin(){
    return finJeu;
}

bool Jeu::deplacePacman(Direction dir)
{
    if (Frame%(10-vitPacman)==0)
    {
        //cout<<"pos pacman ="<<posPacmanX<<","<<posPacmanY<<endl;
        int depX[] = {-1, 1, 0, 0};
        int depY[] = {0, 0, -1, 1};
        int testX, testY;

        testX = posPacmanX + depX[dir];
        testY = posPacmanY + depY[dir];
        if (posPortail(testX,testY))//test si le pacman est sur un portail
        {
            Coord c=Teleportation(testX,testY);
            posPacmanX = c.x;
            posPacmanY = c.y;
            return true;
        }
        else if (posValide(testX, testY))
        {
            posPacmanX = testX;
            posPacmanY = testY;
            return true;
        }
        else
            return false;
    }
    else//Si le pacman ne peut pas se deplacer
        return false;

}

Coord Jeu::Teleportation(int x, int y)
{
    Coord c={-1,-1};
    list<Portail>::iterator itPortail;
	for (itPortail=portail.begin(); itPortail!=portail.end(); itPortail++)
    {
        if ((itPortail->Entree.x == x) && (itPortail->Entree.y == y))
        {
            c=itPortail->Sortie;
        }
    }

    return c;
}
bool Jeu::posPortail(int x, int y)
{
    list<Portail>::iterator itPortail;
	for (itPortail=portail.begin(); itPortail!=portail.end(); itPortail++)
    {
        if ((itPortail->Entree.x == x) && (itPortail->Entree.y == y))
        {
            return true;
        }
    }

    return false;
}
void Jeu::RetraitFantome()
{
    list<Fantome>::iterator itFantome;
    itFantome=fantomes.begin();

    //++itFantome;
    if (itFantome!=fantomes.end())
        fantomes.erase(itFantome);
}

void Jeu::AjoutFantome()
{
    Fantome newFantome;
    int x,y;
    do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        newFantome.posX = x;
        newFantome.posY = y;
        newFantome.dir = (Direction)(rand()%4);

    fantomes.push_back(newFantome);
}
