#include "game.h"
#include <QTime>
#include <QMessageBox>

QPointF dPosition[5]{};

void GameWidget::gameInit() {
    QMessageBox::warning(this, "游戏提示","通过鼠标点击滑动，如果有两个相同的数字即可合并。只要出现了2048即可胜利", "好的");

    srand(unsigned(time(NULL)));
    gameBoard[rand() % 4][rand() % 4] = 2;
    int i = rand() % 4, j = rand() % 4;
    while (gameBoard[i][j] != 0) { i = rand() % 4, j = rand() % 4; } //防止重复位置。
    gameBoard[i][j] = 2;
    update();
}

void GameWidget::playMusic() {
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sounds/bgm.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->setVolume(25);
    music->play();
}

GameWidget::GameWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), timer(this) {
    connect(this, SIGNAL(gestureMove(Direction)), SLOT(onGestureMove(Direction)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    memset(gameBoard, 0, sizeof(gameBoard));
    score = 0;
    digitCount = 2; //初始时候有两个数字。
    isAnimating = false;
    gameInit();
    playMusic();
}

void GameWidget::mousePressEvent(QMouseEvent* e) {
    startPosition = e->pos();
    this->setCursor(Qt::SizeAllCursor);
}
//此函数用以计算鼠标移动轨迹，从而得知移动方向。
void GameWidget::mouseReleaseEvent(QMouseEvent* e) {
    music = new QMediaPlayer;
    music->setMedia(QUrl("qrc:/sounds/move.mp3"));
    music->setVolume(30);
    music->play();
    if (isAnimating) { return; }
    float dX = (float)(e->pos().x() - startPosition.x());
    float dY = (float)(e->pos().y() - startPosition.y());
    if (abs(dX) > abs(dY)) {
        if (dX < 0) { emit gestureMove(D_LEFT); }
        else { emit gestureMove(D_RIGHT); }
    } else {
        if (dY < 0) { emit gestureMove(D_UP); }
        else { emit gestureMove(D_DOWN); }
    }
    this->setCursor(Qt::ArrowCursor);
}

void GameWidget::onGestureMove(Direction direct) {
    int i, j, k;
    Animation ani;
    bool move = false, combine = false; //是否发生合并？
    bool isCombined[4][4];
    memset(isCombined, 0, sizeof(isCombined));
    switch (direct) {
    case D_LEFT:
        for (i = 0; i < 4; i++) {
            j = 0, k = 0;
            while (true) {
                while (j < 4 && gameBoard[i][j] == 0) { j++; }
                if (j > 3) { break; }
                qSwap(gameBoard[i][k], gameBoard[i][j]);
                if (j != k) { move = true; }
                ani.type = MOVE;
                ani.startPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i);
                ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * k, 7 * yDiff + (h + 5 * yDiff) * i);
                ani.digit1 = ani.digit2 = gameBoard[i][k];
                ani.direct = D_LEFT;
                if (k > 0 && gameBoard[i][k] == gameBoard[i][k - 1] && !isCombined[i][k - 1]) {
                    gameBoard[i][k - 1] <<= 1;
                    isCombined[i][k - 1] = true;
                    gameBoard[i][k] = 0;
                    combine = true;
                    ani.digit2 = gameBoard[i][k - 1];
                    ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * (k - 1), 7 * yDiff + (h + 5 * yDiff) * i);
                    score += gameBoard[i][k - 1];
                    emit scoreInc(score);
                    digitCount--;
                } else { k++; }
                j++;
                animationList.append(ani);
            }
        }
        break;
    case D_RIGHT:
        for (i = 0; i < 4; i++) {
            j = 3, k = 3;
            while (true) {
                while (j > -1 && gameBoard[i][j] == 0) { j--; }
                if (j < 0) { break; }
                qSwap(gameBoard[i][k], gameBoard[i][j]);
                if (j != k) { move = true; }
                ani.type = MOVE;
                ani.startPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i);
                ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * k, 7 * yDiff + (h + 5 * yDiff) * i);
                ani.digit1 = ani.digit2 = gameBoard[i][k];
                ani.direct = D_RIGHT;
                if (k < 3 && gameBoard[i][k] == gameBoard[i][k + 1] && !isCombined[i][k + 1]) {
                    gameBoard[i][k + 1] <<= 1;
                    isCombined[i][k + 1] = true;
                    gameBoard[i][k] = 0;
                    combine = true;
                    ani.digit2 = gameBoard[i][k + 1];
                    ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * (k + 1), 7 * yDiff + (h + 5 * yDiff) * i);
                    score += gameBoard[i][k + 1];
                    emit scoreInc(score);
                    digitCount--;
                } else { k--; }
                j--;
                animationList.append(ani);
            }
        }
        break;
    case D_UP:
        for (i = 0; i < 4; i++) {
            j = 0, k = 0;
            while (true) {
                while (j < 4 && gameBoard[j][i] == 0) { j++; }
                if (j > 3) { break; }
                qSwap(gameBoard[k][i], gameBoard[j][i]);
                if (j != k) { move = true; }
                ani.type = MOVE;
                ani.startPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * j);
                ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * k);
                ani.digit1 = ani.digit2 = gameBoard[k][i];
                ani.direct = D_UP;
                if (k > 0 && gameBoard[k][i] == gameBoard[k - 1][i] && !isCombined[k - 1][i]) {
                    gameBoard[k - 1][i] <<= 1;
                    isCombined[k - 1][i] = true;
                    gameBoard[k][i] = 0;
                    combine = true;
                    ani.digit2 = gameBoard[k - 1][i];
                    ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * (k - 1));
                    score += gameBoard[k - 1][i];
                    emit scoreInc(score);
                    digitCount--;
                } else { k++; }
                j++;
                animationList.append(ani);
            }
        }
        break;
    case D_DOWN:
        for (i = 0; i < 4; i++) {
            j = 3, k = 3;
            while (true) {
                while (j > -1 && gameBoard[j][i] == 0) { j--; }
                if (j < 0) { break; }
                qSwap(gameBoard[k][i], gameBoard[j][i]);
                if (j != k) { move = true; }
                ani.type = MOVE;
                ani.startPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * j);
                ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * k);
                ani.digit1 = ani.digit2 = gameBoard[k][i];
                ani.direct = D_DOWN;
                if (k < 3 && gameBoard[k][i] == gameBoard[k + 1][i] && !isCombined[k + 1][i]) {
                    gameBoard[k + 1][i] <<= 1;
                    isCombined[k + 1][i] = true;
                    gameBoard[k][i] = 0;
                    combine = true;
                    ani.digit2 = gameBoard[k + 1][i];
                    ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * i, 7 * yDiff + (h + 5 * yDiff) * (k + 1));
                    score += gameBoard[k + 1][i];
                    emit scoreInc(score);
                    digitCount--;
                } else { k--; }
                j--;
                animationList.append(ani);
            }
        }
        break;
    }

    if ((move || combine) && digitCount != 16){
        i = rand() % 4, j = rand() % 4;
        while (gameBoard[i][j] != 0) { i = rand() % 4, j = rand() % 4; }
        gameBoard[i][j] = (rand() % 2 + 1) * 2;
        ani.type = APPEAR;
        ani.startPosition = ani.endPosition = QPointF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i);
        ani.startPosition += QPointF(w / 2, h / 2);
        ani.digit1 = gameBoard[i][j];
        animationList.append(ani);
        digitCount++;
    }
    isAnimating = true;
    timer.start(10);
}

bool GameWidget::drawAnimation(QPainter &painter) {
    QFont font;
    font.setFamily("helvetica");
    font.setBold(true);
    font.setPixelSize(40 * yDiff);
    painter.setFont(font);
    bool ok = true;

    QBrush brush(QColor::fromRgb(135, 206, 235));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRectF(2 * xDiff, 2 * yDiff, width() - 4 * xDiff, height() - 4 * yDiff), rX, rY);
    brush.setColor(QColor::fromRgb(176, 224, 230));
    painter.setBrush(brush);

    for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) { painter.drawRoundedRect(QRectF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i, w, h), rX, rY); }}
    for (QList<Animation>::iterator it = animationList.begin(); it != animationList.end(); it++) { if (!playAnimation(*it, painter)) { ok = false; }}
    return ok;
}

bool GameWidget::playAnimation(Animation& ani, QPainter& painter) {
    bool ret = false;
    QBrush brush(Qt::SolidPattern);

    if (ani.type == MOVE) {
        switch (ani.direct) {
           case D_LEFT:
               if (ani.startPosition.x() > ani.endPosition.x()) { ani.startPosition += dPosition[D_LEFT]; }
               else { ani.startPosition = ani.endPosition, ret = true; }
               break;
           case D_RIGHT:
               if (ani.startPosition.x() < ani.endPosition.x()) { ani.startPosition += dPosition[D_RIGHT]; }
               else { ani.startPosition = ani.endPosition, ret = true; }
               break;
           case D_UP:
               if (ani.startPosition.y() > ani.endPosition.y()) { ani.startPosition += dPosition[D_UP]; }
               else { ani.startPosition = ani.endPosition, ret = true; }
               break;
           case D_DOWN:
               if (ani.startPosition.y() < ani.endPosition.y()) { ani.startPosition += dPosition[D_DOWN]; }
               else { ani.startPosition = ani.endPosition, ret = true; }
           }

        if (!ret) {
            brush.setColor(digitBackground[getColor(ani.digit1)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(ani.startPosition.x(), ani.startPosition.y(), w, h), rX, rY);
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(ani.startPosition.x(), ani.startPosition.y(), w, h), Qt::AlignCenter, QString::number(ani.digit1));
        } else {
            brush.setColor(digitBackground[getColor(ani.digit2)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(ani.startPosition.x(), ani.startPosition.y(), w, h), rX, rY);
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(ani.startPosition.x(), ani.startPosition.y(), w, h), Qt::AlignCenter, QString::number(ani.digit2));
        }
        painter.setPen(Qt::NoPen);
    } else {
        if (ani.startPosition.x() > ani.endPosition.x()) { ani.startPosition += dPosition[4]; }
        else { ani.startPosition = ani.endPosition, ret = true; }
        brush.setColor(digitBackground[getColor(ani.digit1)]);
        painter.setBrush(brush);
        painter.drawRoundedRect(QRectF(ani.startPosition.x(), ani.startPosition.y(), w - 2 * (ani.startPosition.x() - ani.endPosition.x()), h - 2 * (ani.startPosition.y() - ani.endPosition.y())), rX, rY);
        painter.setPen(QColor::fromRgb(0, 0, 0));
        painter.drawText(QRectF(ani.endPosition.x(), ani.endPosition.y(), w, h), Qt::AlignCenter, QString::number(ani.digit1));
        painter.setPen(Qt::NoPen);
    }
    return ret;
}

void GameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (isAnimating) {
        if (drawAnimation(painter)) {
            isAnimating = false;
            timer.stop();
            animationList.clear();
            if (digitCount == 16) {
                if (isGameOver()) {
                    emit gameOver();
                    restart();
                }
            }
            if (isGameWin()) {
                emit win();
                restart();
            }
        }
        return;
    }

    QBrush brush(QColor::fromRgb(145, 206, 235));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRectF(2 * xDiff, 2 * yDiff, width() - 4 * xDiff, height() - 4 * yDiff), rX, rY);

    QFont font;
    font.setFamily("helvetica");
    font.setBold(true);
    font.setPixelSize(40 * yDiff);

    painter.setFont(font);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (gameBoard[i][j]) {
                brush.setColor(digitBackground[getColor(gameBoard[i][j])]);
                painter.setBrush(brush);
                painter.drawRoundedRect(QRectF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i, w, h), rX, rY);
                painter.setPen(QColor::fromRgb(0, 0, 0));
                painter.drawText(QRectF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i, w, h), Qt::AlignCenter, QString::number(gameBoard[i][j]));
                painter.setPen(Qt::NoPen);
            } else {
                brush.setColor(QColor::fromRgb(176, 224, 230));
                painter.setBrush(brush);
                painter.drawRoundedRect(QRectF(7 * xDiff + (w + 5 * xDiff) * j, 7 * yDiff + (h + 5 * yDiff) * i, w, h), rX, rY);
            }
        }
}
//重新计算位置
void GameWidget::resizeEvent(QResizeEvent *) {
    xDiff = width() / 400.0, yDiff = height() / 400.0;
    w = width() - 4 * xDiff, h = height() - 4 * yDiff;
    w = (w - 25 * xDiff) / 4, h = (h - 25 * yDiff) / 4;
    rX = 15 * xDiff, rY = 15 * yDiff;
    dPosition[0] = QPointF(-25 * xDiff, 0);
    dPosition[1] = QPointF(25 * xDiff, 0);
    dPosition[2] = QPointF(0, -25 * yDiff);
    dPosition[3] = QPointF(0, 25 * yDiff);
    dPosition[4] = QPointF(-4 * xDiff, -4 * yDiff);
}

void GameWidget::restart() {
    score = 0;
    digitCount = 2;
    memset(gameBoard, 0, sizeof(gameBoard));
    gameInit();
    emit scoreInc(score);
    update();
}

bool GameWidget::isGameOver() {
    /*dfs做法，会更加节省时间。
     * const int dx[] = {0, 1, 0, -1};
     * const int dy[] = {1, 0, -1, 0};
     * QQueue<QPair<int, int>> q.
     * q.enqueue(qMakePair(0, 0));
     *
     * while (!q.empty()) {
     *      int s = q.size();
     *      for (int i = 0; i < s; ++i) {
     *          auto front = q.front();
     *             for (int j = 0; j < 4; j++) {
     *                  int nx = front.first + dx[j];
     *                  int ny = front.second + dy[j];
     *                  if (nx < 4 && ny < 4 && nx >= 0 && ny >= 0) {
     *                      if (gameBoard[front.first][front.second] == gameBoard[nx][ny]) { return false; }
     *                      q.enqueue(qMakePair(nx, ny));
     *                  }
     *             }
     *      }
     *  }
     * }
     */

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((j != 3 && gameBoard[i][j] == gameBoard[i][j + 1]) ||
                (i != 3 && gameBoard[i][j] == gameBoard[i + 1][j]))
            { return false; }
        }
    }
    return true;
}

bool GameWidget::isGameWin() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameBoard[i][j] == 2048) { return true; }
        }
    }
    return false;
}

int GameWidget::getColor(int n) {
    int cnt = 0;
    while (n >>= 1) { cnt++; }
    return cnt - 1;
}
