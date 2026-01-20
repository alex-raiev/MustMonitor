#include "udpreader.h"
#include <QNetworkDatagram>
#include <iostream>

UdpReader::UdpReader(ThreadSafeQueue<QString>* queue, QObject *parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
    , m_queue(queue)
{
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpReader::readPendingDatagrams);
}

void UdpReader::start(int port) {
    m_socket->bind(QHostAddress::Any, port);
}

void UdpReader::readPendingDatagrams() {
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        QString data = QString::fromUtf8(datagram.data());
        std::cout << "UDP received: " << data.toStdString() << std::endl;
        m_queue->push(data);
    }
}
