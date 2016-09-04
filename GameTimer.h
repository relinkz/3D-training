#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
public:
	GameTimer();
	virtual ~GameTimer();

	float GameTime() const;		//in secounds
	float DeltaTime() const;	//in secounds

	void Reset();				//call before message loop
	void Start();				//call when unpaused
	void Stop();				//call when paused
	void Tick();				//call every frame
};

#endif