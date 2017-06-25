#include <QCoreApplication>
#include <QtTest>

#include "../../tests/single_participant.h"
#include "../../tests/multiple_participants.h"

int main(int argc, char *argv[])
{
    // Define function for running the tests.
    int status = 0;
    auto ExecuteTest = [ &status, argc, argv ]( QObject* obj ) -> void {
        status |= QTest::qExec( obj, argc, argv );
        delete obj;
    };

    // Test execution.
    ExecuteTest( new gp::tests::single_participant() );
    ExecuteTest( new gp::tests::multiple_participants() );

    return status;
}
