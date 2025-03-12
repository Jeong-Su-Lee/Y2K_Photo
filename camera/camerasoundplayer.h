#ifndef CAMERASOUNDPLAYER_H
#define CAMERASOUNDPLAYER_H

#include <QObject>

class CameraSoundPlayer : public QObject
{
    Q_OBJECT

public:
    explicit CameraSoundPlayer(QObject *parent = nullptr);
    void startCameraSound();
};

#endif // CAMERASOUNDPLAYER_H
