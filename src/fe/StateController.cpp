#include <iostream>
#include "fe/StateController.h"

using namespace std;

namespace fe
{

// =============================================================================
//	CONSTRUCTORS, COPY CONSTRUCTOR, DESTRUCTOR, ASSIGNMENT OPERATOR
// =============================================================================
StateController::StateController() :
	m_State(TInternalState::STOPPED),
	m_RunCallback(nullptr),
	m_PauseCallback(nullptr),
	m_UnpauseCallback(nullptr),
	m_StopCallback(nullptr),
	m_SetCallbacksOnlyOnce(false)
{
}

StateController::StateController(bool setCallbacksOnlyOnce) :
	m_State(TInternalState::STOPPED),
	m_RunCallback(nullptr),
	m_PauseCallback(nullptr),
	m_UnpauseCallback(nullptr),
	m_StopCallback(nullptr),
	m_SetCallbacksOnlyOnce(setCallbacksOnlyOnce)
{
}

StateController::~StateController()
{
}

// =============================================================================
//	REGULAR METHODS
// =============================================================================
bool StateController::Run()
{
	if(m_State != TInternalState::STOPPED)
		return false;

	m_State = TInternalState::RUNNING;

	if(m_RunCallback != nullptr)
		m_RunCallback();

	return true;
}

bool StateController::Pause()
{
	if(m_State != TInternalState::RUNNING)
		return false;

	m_State = TInternalState::PAUSED;

	if(m_PauseCallback != nullptr)
		m_PauseCallback();

	return true;
}

bool StateController::Unpause()
{
	if(m_State != TInternalState::PAUSED)
		return false;

	m_State = TInternalState::RUNNING;

	if(m_UnpauseCallback != nullptr)
		m_UnpauseCallback();

	return true;
}

bool StateController::Stop()
{
	if(m_State == TInternalState::STOPPED)
		return false;

	m_State = TInternalState::STOPPED;

	if(m_StopCallback != nullptr)
		m_StopCallback();

	return true;
}

// =============================================================================
//	GETTERS & SETTERS
// =============================================================================
void StateController::SetRunCallback(const Callback& callback)
{
	if(m_SetCallbacksOnlyOnce && m_RunCallback != nullptr)
	{
		cerr << "Setting callbacks more than once is forbbiden" << endl;
		return;
	}

	m_RunCallback = callback;
}

void StateController::SetPauseCallback(const Callback& callback)
{
	if(m_SetCallbacksOnlyOnce && m_PauseCallback != nullptr)
	{
		cerr << "Setting callbacks more than once is forbbiden" << endl;
		return;
	}

	m_PauseCallback = callback;
}

void StateController::SetUnpauseCallback(const Callback& callback)
{
	if(m_SetCallbacksOnlyOnce && m_UnpauseCallback != nullptr)
	{
		cerr << "Setting callbacks more than once is forbbiden" << endl;
		return;
	}

	m_UnpauseCallback = callback;
}

void StateController::SetStopCallback(const Callback& callback)
{
	if(m_SetCallbacksOnlyOnce && m_StopCallback != nullptr)
	{
		cerr << "Setting callbacks more than once is forbbiden" << endl;
		return;
	}

	m_StopCallback = callback;
}

// =============================================================================
//	QUESTION METHODS
// =============================================================================
bool StateController::IsRunning() const { return m_State == TInternalState::RUNNING; }
bool StateController::IsPaused() const  { return m_State == TInternalState::PAUSED;  }
bool StateController::IsStopped() const { return m_State == TInternalState::STOPPED; }

}
