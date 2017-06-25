#ifndef TESTD_H
#define TESTD_H

#include <QObject>

class testd : public QObject
{
    Q_OBJECT
public:
    explicit testd(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TESTD_H