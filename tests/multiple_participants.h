#ifndef MULTIPLE_PARTICIPANTS_H
#define MULTIPLE_PARTICIPANTS_H

#include <QtTest>

namespace gp
{
namespace tests
{

class multiple_participants : public QObject
{
    Q_OBJECT
public:
    explicit multiple_participants(QObject *parent = nullptr);

signals:

private slots:

    //! Verifies that multiple participants can deallocate objects.
    void deallocationSucceedsFromMultipleThreadsSucceeds();

    //! Verifies that deallocating objects after they have been shared between threads succeeeds.
    void deallocationAfterExchangingObjectsBetweenThreadsSucceeds();

};

}
}
#endif // MULTIPLE_PARTICIPANTS_H
