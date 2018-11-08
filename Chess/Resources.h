#pragma once

#include <map>
#include <string>
#include "Sprite.h"

class Resources
{
    public:
        Resources();
        ~Resources();
        Resources(const Resources & other);
        Resources& operator=(const Resources& other);
    private:
        std::map<std::string, Sprite> sprites;
		void LoadResource(const std::string& tag, const std::string& path);
    public:
        Sprite GetSprite(const std::string& tag) const;
};
