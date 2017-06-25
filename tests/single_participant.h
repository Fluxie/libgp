#ifndef GB_TESTS_SINGLE_PARTICIPANT_H
#define GB_TESTS_SINGLE_PARTICIPANT_H

#include <QtTest>

namespace gp
{
namespace tests
{

class single_participant : public QObject
{
    Q_OBJECT

private slots:

    //! Verifies that deallocating an object succeeds.
    void deallocatingOneObjectSucceeds();

    //! Verifies that deleting an object succeeds.
    void deletingOneObjectSucceeds();

    //! Verifies that deleting an object succeeds.
    void deletingMultipleObjectsSucceeds();

    //! Verifies that mixing deallocation types succeeds.
    void mixingDeallocationTypesSucceeds();

};

}
}



#endif // GB_TESTS_SINGLE_PARTICIPANT_H
