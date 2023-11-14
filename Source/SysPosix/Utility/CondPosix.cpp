
#include "Base/Types.h"
#include "System/Condition.h"
#include "System/Mutex.h"

#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <chrono> 

const double kTimeoutInfinity = 0.f;

// Cond wrapper derived from GLFW 2.7, see http://www.glfw.org/.

Cond * CondCreate()
{
	pthread_cond_t * cond = (pthread_cond_t *)malloc( sizeof(pthread_cond_t) );
	if (!cond)
	{
		return NULL;
	}

	pthread_cond_init( cond, NULL );
	return (Cond *)cond;
}

void CondDestroy(Cond * cond)
{
	// pthread_cond_destroy( (pthread_cond_t *)cond );
	free( cond );
}
void ComputeWait(double timeout, timespec* wait)
{
    auto current_time_point = std::chrono::system_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::microseconds(static_cast<long long>(timeout * 1000000.0))
    );

    auto new_time_point = current_time_point + duration_sec;

    auto seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(
        new_time_point.time_since_epoch()
    );

    auto remaining_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
        new_time_point - std::chrono::time_point_cast<std::chrono::seconds>(new_time_point)
    );

    wait->tv_sec = seconds_since_epoch.count();
    wait->tv_nsec = remaining_nanoseconds.count();
}

void CondWait(Cond * cond, Mutex * mutex, double timeout)
{
	if (timeout <= 0)
	{
		// pthread_cond_wait( (pthread_cond_t *)cond, &mutex->mMutex );
	}
	else
	{
		timespec wait;
		ComputeWait(timeout, &wait);

		// pthread_cond_timedwait( (pthread_cond_t *)cond, &mutex->mMutex, &wait );
	}
}

void CondSignal(Cond * cond)
{
	pthread_cond_signal( (pthread_cond_t *)cond );
}
