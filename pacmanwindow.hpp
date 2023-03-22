#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapFantome, pixmapMur, pixmapVide;
    QPixmap pixmapPacman [3];
    int imagePacman;
    int etatJeu;//1 si le jeu est en fonction et 0 si la fin du jeu

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimer();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void ajoutFantome();
    void retraitFantome();
    void finDeJeu();
    void onGameOver();
};

class PacmanButton : public QPushButton
{
    public:
        PacmanButton(const QString &text, QWidget *parent = nullptr);
        void keyPressEvent(QKeyEvent *);
};

#endif
