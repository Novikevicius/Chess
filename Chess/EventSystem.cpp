#include <iostream>
#include "EventSystem.h"
#include <algorithm>

using namespace std;

EventSystem* EventSystem::instance = nullptr;
Map EventSystem::listeners;

EventSystem* EventSystem::GetInstance()
{
	if (!instance)
		instance = new EventSystem();
	return instance;
}
void EventSystem::RegisterEvent(Listener* l, Event e)
{
    if(!l)
        throw "Listener cannot be null";
    if(AlreadyRegistered(l, e))
        throw "Listener is already registered to this event";
	listeners.insert(make_pair(e, l));
}
void EventSystem::UnregisterEvent(Listener* l, Event e)
{
	const auto range = listeners.equal_range(e);
	auto it = range.first;
	while (it != range.second)
		if (it->second == l)
			it = listeners.erase(it);
		else
			++it;
}
void EventSystem::UnregisterEvent(Listener* l)
{
	auto it = listeners.begin();
	while ( it != listeners.end())
		if (it->second == l)
			it = listeners.erase(it);
		else
			++it;
}
void EventSystem::UnregisterAll()
{
	listeners.clear();
}
void EventSystem::Invoke(Event e)
{
	const auto range = listeners.equal_range(e);
	for (auto it = range.first; it != range.second; ++it)
		it->second->HandleEvent(e);
}
bool EventSystem::AlreadyRegistered(Listener* l, Event e)
{
	const auto range = listeners.equal_range(e);
	for (auto it = range.first; it != range.second; ++it)
		if (it->second == l)
			return true;
    return false;
}
