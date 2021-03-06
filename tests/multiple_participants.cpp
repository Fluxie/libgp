/*
* libgp - A library implementing Quiescent State Based Reclamanation
* Copyright (C) 2017  Juha Lepola
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "multiple_participants.h"

#include "../src/gp.h"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

namespace gp
{
namespace tests
{

multiple_participants::multiple_participants(QObject *parent) : QObject(parent)
{

}

//! Verifies that multiple participants can deallocate objects.
void multiple_participants::deallocationSucceedsFromMultipleThreadsSucceeds()
{
    gp::unregister_thread();

    // Prepare synchronization primitives to ensure all the participants
    // have created their objects before the cleanup.
    std::mutex deallocationGuard;
    size_t readCount = 0;
    size_t safeZoneUpdates = 0;
    std::condition_variable deallocationVariable;

    // Start threads based on the HW support.
    size_t participantCount = std::thread::hardware_concurrency();
    std::vector< std::thread > participants;
    participants.reserve( participantCount );
    for( size_t tCount = 0; tCount < participantCount; ++tCount )
    {
        participants.emplace_back( [&] () -> void
        {
            // Create an object.
            std::string* hello = new std::string( "Hello" );

            // Mark object for deallocation.
            gp::pool().deallocate( hello );

            // Wait for the deallocations to complete.
            {
                std::unique_lock< std::mutex > lock( deallocationGuard );

                readCount++;

                deallocationVariable.wait( lock, [ &readCount, &participantCount ] () -> bool { return readCount == participantCount; } );

                // Wunregister_participante have finished so everybody else must be too.
                deallocationVariable.notify_all();
            }

            gp::pool().safe_zone();

            // Wait for safe-zone updates to complete to ensure
            // we can clean all allocated objects immediately.
            {
                std::unique_lock< std::mutex > lock( deallocationGuard );

                safeZoneUpdates++;

                deallocationVariable.wait( lock, [ &safeZoneUpdates, &participantCount ] () -> bool
                        { return safeZoneUpdates == participantCount; } );

                // We have finished so everybody else must be too.
                deallocationVariable.notify_all();
            }

            // Run cleanup routines.
            bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
            QVERIFY( allDeallocated );

        } );
    }

    // Wait for all the threads to finish.
    for( std::thread& p : participants )
        p.join();

    gp::register_thread();
}

//! Verifies that deallocating objects after they have been shared between threads succeeeds.
void multiple_participants::deallocationAfterExchangingObjectsBetweenThreadsSucceeds()
{
    gp::unregister_thread();

    // Prepare synchronization primitives to ensure all the participants
    // have created their objects before the cleanup.
    std::mutex progressGuard;
    size_t objectsCreated = 0;
    bool dataShuffled= false;
    size_t objectsDeallocated = 0;
    size_t safeZoneUpdates = 0;
    std::condition_variable progress;

    // Prepare buffer for suffling the variables.
    size_t participantCount = std::thread::hardware_concurrency();
    std::vector< std::string* > exchangedStrings( participantCount, nullptr );

    // Create thread according to HW support.
    std::vector< std::thread > participants;
    participants.reserve( participantCount );
    for( size_t tCount = 0; tCount < participantCount; ++tCount )
    {
        participants.emplace_back( [&, tCount] () -> void
        {
            // Id of the current thread.
            size_t id = tCount;

            // Create an object.
            exchangedStrings[ id ] = new std::string( "Hello" );

            // Wait for the creations to complete.
            {
                std::unique_lock< std::mutex > lock( progressGuard );

                objectsCreated++;

                progress.wait( lock, [ &objectsCreated, &participantCount ] () -> bool { return objectsCreated == participantCount; } );

                progress.notify_all();
            }

            // Wait for the main thread to shuffle the values.
            {
                std::unique_lock< std::mutex > lock( progressGuard );

                progress.wait( lock, [ &dataShuffled ] () -> bool { return dataShuffled; } );
            }

            // Read the string.
            QVERIFY( (*exchangedStrings[ id ]) == "Hello" );

            // Mark object for deallocation.
            gp::pool().deallocate( exchangedStrings[ id ] );

            // Wait for deallocations to complete
            {
                std::unique_lock< std::mutex > lock( progressGuard );

                objectsDeallocated++;

                progress.wait( lock, [ &objectsDeallocated, &participantCount ] () -> bool
                        { return objectsDeallocated == participantCount; } );

                // We have finished so everybody else must be too.
                progress.notify_all();
            }

            gp::safe_zone();

            // Wait for safe-zone updates to complete to ensure
            // we can clean all allocated objects immediately.
            {
                std::unique_lock< std::mutex > lock( progressGuard );

                safeZoneUpdates++;

                progress.wait( lock, [ &safeZoneUpdates, &participantCount ] () -> bool
                        { return safeZoneUpdates == participantCount; } );

                // We have finished so everybody else must be too.
                progress.notify_all();
            }

            // Run cleanup routines.
            bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
            QVERIFY( allDeallocated );

        } );
    }

    // Wait for the threads to finish creating the values.
    {
        std::unique_lock< std::mutex > lock( progressGuard );

        progress.wait( lock, [ &objectsCreated, &participantCount ] () -> bool { return objectsCreated == participantCount; } );

        // Shuffle the values.
        std::random_device rd;
        std::mt19937 g( rd() );
        std::shuffle( exchangedStrings.begin(), exchangedStrings.end(), g );
        dataShuffled = true;
    }

    progress.notify_all();

    // Wait for all the threads to finish.
    for( std::thread& p : participants )
        p.join();

    gp::register_thread();
}

}

}
