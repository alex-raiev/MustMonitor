#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include "threadsafequeue.h"

class UdpReader : public QObject {
    Q_OBJECT
    
public:
    explicit UdpReader(ThreadSafeQueue<QString>* queue, QObject *parent = nullptr);
    
    Q_INVOKABLE void start(int port = 9999);
    
private slots:
    void readPendingDatagrams();
    
private:
    QUdpSocket *m_socket;
    ThreadSafeQueue<QString>* m_queue;
};
