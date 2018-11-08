#pragma once

#include "Events.h"

class Event
{
    public:
        Event(const Events = static_cast<Events>(0));
		Event& operator=(const Event& e);
		const bool operator==(const Event& e) const;
		bool operator < (const Event& e) const;
    private:
        Events id;
};
