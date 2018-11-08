#pragma once

#include "Event.h"

class Listener
{
    public:
        Listener();
        virtual ~Listener();
    public:
		virtual void HandleEvent(const Event& e) = 0;
};
