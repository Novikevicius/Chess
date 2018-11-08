#include "Event.h"

Event::Event(const Events v)
{
    id = v;
}
Event & Event::operator=(const Event & e)
{
	id = e.id;
	return *this;
}
const bool Event::operator==(const Event& e) const
{
    return id == e.id;
}

bool Event::operator<(const Event& e) const
{
	return id < e.id;
}
