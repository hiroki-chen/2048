#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QAction>
#include <QMediaPlayer>
#include <QGLWidget>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QList>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMenuBar>
#include <QMenu>
#include <qmath.h>

const QColor digitBackground[11] = {
    QColor::fromRgb(255, 239, 213), QColor::fromRgb(255, 222, 173),
    QColor::fromRgb(255, 218, 185), QColor::fromRgb(255, 106, 106),
    QColor::fromRgb(251, 90, 99), QColor::fromRgb(205, 85, 85),
    QColor::fromRgb(255, 130, 71), QColor::fromRgb(238, 121, 66),
    QColor::fromRgb(205, 104, 57), QColor::fromRgb(255, 140, 0),
    QColor::fromRgb(255, 127, 80)
};

enum Direction {
    D_LEFT = 0,
    D_RIGHT = 1,
    D_UP = 2,
    D_DOWN = 3
};

enum AnimationType {
    MOVE = 0,
    APPEAR = 1
};

struct Animation {
    AnimationType type;
    Direction direct;
    QPointF startPosition;
    QPointF endPosition;
    int digit1;
    int digit2;
};

class GameWidget : public QGLWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

private:
    QMediaPlaylist* playlist;
    QMediaPlayer* music;
    int gameBoard[4][4];
    int digitCount;
    int score;
    QPoint startPosition;
    QList<Animation> animationList;

    qreal w, h;
    qreal xDiff, yDiff;
    qreal rX, rY;
    bool isAnimating;
    QTimer timer;

    void gameInit();
    void playMusic();

    bool isGameOver();
    bool isGameWin();

    int getColor(int);

    bool playAnimation(Animation&, QPainter&);
    bool drawAnimation(QPainter&);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
signals:
    void gestureMove(Direction);
    void scoreInc(int);
    void gameOver();
    void win();

public slots:
    void onGestureMove(Direction);
    void restart();
};

#endif
