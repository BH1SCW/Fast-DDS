/*************************************************************************
 * Copyright (c) 2016 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file AsyncWriterThread.h
 *
 */
#ifndef _RTPS_RESOURCES_ASYNCWRITERTHREAD_H_
#define _RTPS_RESOURCES_ASYNCWRITERTHREAD_H_

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <list>

namespace boost
{
    class thread;
}

namespace eprosima
{
namespace fastrtps
{
namespace rtps
{
   class RTPSWriter;

   /*!
    * @brief This singleton owns a thread that manages asynchronous writes.
    * Asynchronous writes happen directly (when using an async writer) and
    * indirectly (when responding to a NACK).
    */
   class AsyncWriterThread
   {
       public:
           /*!
            * @brief Returns the only singleton instance of AsyncWriterThread.
            */
           static AsyncWriterThread* instance();

           /*!
            * @brief This function adds a writer to be managed by this thread.
            * Only asynchronous writers are permitted.
            * @param writer Asynchronous writer to be added. 
            * @return Result of the operation.
            */
           bool addWriter(RTPSWriter& writer);

           /*!
            * @brief This function removes a writer.
            * @param writer Asynchronous writer to be removed.
            * @return Result of the operation.
            */
           bool removeWriter(RTPSWriter& writer);

           /*
            * Wakes the thread up.
            * @param interested
            */
           void wakeUp();
           //void wakeUp(RTPSParticipant* interestedParticipant);
           //void wakeUp(RTPSWriter * interestedWriter);

           /*!
            * @brief Destructor
            * The destructor is not thread-safe.
            */
           ~AsyncWriterThread();

       private:

           /*!
            * @brief Default constructor.
            */
           AsyncWriterThread();

            //! Singleton, hence no copy.
           AsyncWriterThread(const AsyncWriterThread&) = delete;
           
            //! Singleton, hence no assignment.
           const AsyncWriterThread& operator=(const AsyncWriterThread&) = delete;

           //! @brief runs main method
           void run();

           //! Thread
           std::thread* thread_;

           //! Mutex
           std::mutex mutex_;
           
           static AsyncWriterThread* instance_;

           //! List of asynchronous writers.
           std::list<RTPSWriter*> async_writers;

           std::atomic<bool> running_;
           std::atomic<bool> run_scheduled_;
           std::condition_variable cv_;
   };
} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif // _RTPS_RESOURCES_ASYNCWRITERTHREAD_H_
