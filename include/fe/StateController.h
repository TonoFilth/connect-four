#ifndef __STATE_CONTROLLER_H__
#define __STATE_CONTROLLER_H__

#include <functional>

namespace fe
{

typedef std::function<void()> Callback;

enum class TInternalState
{
	RUNNING,
	PAUSED,
	STOPPED
};

class StateController
{
private:
	TInternalState m_State;

	Callback m_RunCallback;
	Callback m_PauseCallback;
	Callback m_UnpauseCallback;
	Callback m_StopCallback;

	bool m_SetCallbacksOnlyOnce;

public:
	StateController();
	StateController(bool setCallbacksOnlyOnce);
	StateController(const StateController& toCopy);
	StateController& operator=(const StateController& toCopy);
	~StateController();

	bool Run();
	bool Pause();
	bool Unpause();
	bool Stop();

	void SetRunCallback(const Callback& callback);
	void SetPauseCallback(const Callback& callback);
	void SetUnpauseCallback(const Callback& callback);
	void SetStopCallback(const Callback& callback);

	bool IsRunning() const;
	bool IsPaused() const;
	bool IsStopped() const;
};

}

#endif
