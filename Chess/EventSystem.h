#pragma once

#include <map>
#include "Event.h"
#include "Listener.h"

using Map = std::multimap<Event, Listener*>;

class EventSystem
{
    private:
		EventSystem() = default;
		EventSystem(const EventSystem& s) = delete;
		EventSystem operator=(const EventSystem& rhs) = delete;
    private:
		static EventSystem* instance;
		static Map listeners;
    private:
        // Misc functionsc
		static bool AlreadyRegistered(Listener* l, Event e);
    public:
		static EventSystem* GetInstance();
        void RegisterEvent(Listener* l, Event e);
        void UnregisterEvent(Listener* l, Event e);
        void UnregisterEvent(Listener* l);
        void UnregisterAll();
        void Invoke(Event e);
};
