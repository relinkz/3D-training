#include "GameTimer.h"
#include <Windows.h>



GameTimer::GameTimer()
{
	this->mSecondsPerCount	= 0.0f;
	this->mDeltaTime		= 0.0f;

	this->mBaseTime			= 0;
	this->mPausedTime		= 0;
	this->mStopTime			= 0;
	this->mPrevTime			= 0;
	this->mCurrTime			= 0;

	this->mStopped			= false;

	__int64 countPerSec = 0;
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);
	this->mSecondsPerCount = 1.0f / (double)countPerSec;
}


GameTimer::~GameTimer()
{
}

float GameTimer::GameTime() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if (this->mStopped == true)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime() const
{
	return static_cast<float>(this->mDeltaTime);
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	this->mBaseTime = currTime;
	this->mPrevTime = currTime;

	this->mStopTime = 0;
	this->mStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if (this->mStopped)
	{
		this->mPausedTime += (startTime - mStopTime);

		this->mPrevTime = startTime;
		this->mStopTime = 0;
		this->mStopped = false;
	}
}

void GameTimer::Stop()
{
	if (this->mStopped == false)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		this->mStopTime = currTime;
		this->mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (this->mStopped == true)
	{
		this->mDeltaTime = 0.0;
		return;
	}
	// get the time this frame
	__int64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	this->mCurrTime = currTime;

	//time difference between this frame and the previous
	this->mDeltaTime = (this->mCurrTime - this->mPrevTime) * this->mSecondsPerCount;

	//prepare for next frame
	this->mPrevTime = this-> mCurrTime;

	/*
	Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
	processor goes into power save mode or we get shuffled to another
	processor, the mDeltaTime can be negative
	*/
	if (this->mDeltaTime < 0.0f)
	{
		this->mDeltaTime = 0.0;
	}
}
