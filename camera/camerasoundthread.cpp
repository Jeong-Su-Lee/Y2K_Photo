#include "camerasoundthread.h"

CameraSoundThread::CameraSoundThread(QObject *parent)
    : QThread(parent)
{
}

CameraSoundThread::~CameraSoundThread()
{
}

void CameraSoundThread::run()
{
    QProcess *setupProcess = new QProcess();
    QProcess *playProcess = new QProcess();

    // Run the setup commands
    setupProcess->start("sh", QStringList() << "-c" << "cd /mnt/nfs && source ./alsa.sh && ./aplay -l && ./amixer -c 0 cset numid=1 80%");
    setupProcess->waitForFinished();
    delete setupProcess;

    // Start the aplay process
    playProcess->start("sh", QStringList() << "-c" << "./aplay -Dhw:0,0 /mnt/nfs/test_contents/camera_capture_1.wav");

    // Create a timer to terminate the play process after 3 seconds
    QTimer timer;
    connect(&timer, &QTimer::timeout, playProcess, &QProcess::terminate);
    timer.start(2000);

    // Use an event loop to wait for the timer and process to finish
    QEventLoop loop;
    connect(playProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();

    // Wait for the play process to finish
    playProcess->waitForFinished();

    delete playProcess;
}
