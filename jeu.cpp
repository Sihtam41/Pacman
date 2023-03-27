#include <iostream>
#include <cassert>
#include <algorithm>
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

bool Jeu::init()
{
	int x, y;
	list<Fantome>::iterator itFantome;

	const char terrain_defaut[15][21] = {
		"####################",
		"#........##........#",
		"#.#####..##...####.#",
		"#.....#..##........#",
		"#..##.#............#",
		"#...........#......#",
		"#......#...##......#",
		"#####..#....#..#####",
		"#......##...#......#",
		"#......#....#......#",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####################"
    };
    /*const char terrain_defaut[25][41] = {
        "#########################.#............#",
        "#...............#.......#.#.##########.#",
        "#.#########.###.#.#####.#.#...........#.",
        "#.#.......#.#...#.#.....#.#.#########.#.",
        "#.#.#####.#.#.#.#.#####.#.#.#.......#.#.",
        "#.#.#.....#.#.#.#...#...#.#.#.#.#.#.#.#.",
        "#.#.#.#####.#.#.#.###.#.#.#.#.#.#.#.#.#.",
        "#.#.#.#.....#.#.#.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#.#.#.#####.#.#.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#.#.#.......#.#.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#.#.#########.#.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#.#...........#.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#.#############.#.#.#.#.#.#.#.#.#.#.#.",
        "#.#...............#.#.#.#.#.#.#.#.#.#.#.",
        "#.#################.#.#.#.#.#.#.#.#.#.#.",
        "#...................#.#.#.#.#.#.#.#.#.#.",
        "#####################.#.#.#.#.#.#.#.#.#.",
        "#.....................#.#.#.#.#.#.#.#.#.",
        "#.#####################.#.#.#.#.#.#.#.#.",
        "#.#...................#.#.#.#.#.#.#.#.#.",
        "#.#.#################.#.#.#.#.#.#.#.#.#.",
        "#.#.................#.#.#.#.#.#.#.#.#.#.",
        "#.#################.#.#.#.#.#.#.#.#.#.#.",
        "#.................#.#.#.#.#.#.#.#.#.#.#.",
        "##################.#.#.#.#.#.#.#.#.#.#.."
    };*/
       /* const char terrain_defaut[15][22] = {
        "#####################",
        "#.............#.....#",
        "#.#.###.###.#.#..##.#",
        "#.#.#...#...#.##.#..#",
        "#.#.#.###.#.#.##.#.##",
        "#.........#.#....#.##",
        "#####.#.#.#.#.#.#...#",
        "#.....#.#.#.#.#.#.#.#",
        "#####.#...#.#.#.#.#.#",
        "#.....#.###.........#",
        "#.#####.#.#.#.##.##.#",
        "#.......#.#.#.##....#",
        "#.#####.#.#.#.##.##.#",
        "#.#.................#",
        "#####################"

    };*/


	largeur = 20;
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

    fantomes.resize(4);//définit le nombre de fantomes au lacement du niveau

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    posPacmanX = x,
    posPacmanY = y;

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

                    else if (itFantome->comportement==FUYARD)// ne fonctionne pas bien
                    {
                        auto it_DROITE = find(Dir_test.begin(), Dir_test.end(), DROITE);
                        auto it_GAUCHE = find(Dir_test.begin(), Dir_test.end(), GAUCHE);
                        auto it_HAUT = find(Dir_test.begin(), Dir_test.end(), HAUT);
                        auto it_BAS = find(Dir_test.begin(), Dir_test.end(), BAS);
                        // Déplacement loin du pacman
                        if (itFantome->posX<posPacmanX && it_GAUCHE==Dir_test.end())
                            itFantome->dir = GAUCHE;
                        else if (itFantome->posX>posPacmanX && it_DROITE==Dir_test.end())
                            itFantome->dir = DROITE;
                        else if (itFantome->posY<posPacmanY && it_HAUT==Dir_test.end())
                            itFantome->dir = HAUT;
                        else if (itFantome->posY>posPacmanY && it_BAS==Dir_test.end())
                            itFantome->dir = BAS;
                        else
                            itFantome->dir = (Direction)(rand()%4);

                        
                        Dir_test.push_back(itFantome->dir);
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
                        auto path = generator.findPath({itFantome->posX,itFantome->posY}, {posPacmanX, posPacmanY});
                        if (path.size()>1)
                        {
                            int curseurProchainDeplacement= path.size()-2;
                            if (path[curseurProchainDeplacement].x>itFantome->posX)
                                itFantome->dir = DROITE;
                            else if (path[curseurProchainDeplacement].x<itFantome->posX)
                                itFantome->dir = GAUCHE;
                            else if (path[curseurProchainDeplacement].y>itFantome->posY)
                                itFantome->dir = BAS;
                            else if (path[curseurProchainDeplacement].y<itFantome->posY)
                                itFantome->dir = HAUT;
                        }
                        else
                        {
                            itFantome->dir = (Direction)(rand()%4);
                            cout<<"bug : Le Traqueur ne trouve pas de chemin"<<endl;
                        }
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

                if (terrain[testY*largeur+testX]==VIDE)//si le fantome ne va pas sur un mur
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
        int depX[] = {-1, 1, 0, 0};
        int depY[] = {0, 0, -1, 1};
        int testX, testY;

        testX = posPacmanX + depX[dir];
        testY = posPacmanY + depY[dir];

        if (posValide(testX, testY))
        {
            posPacmanX = testX;
            posPacmanY = testY;
            return true;
        }
        else
            return false;
    }
    else
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
