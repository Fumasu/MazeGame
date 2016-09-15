#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <map>
#include "Resources/Text.hpp"
#include <Poco/Logger.h>

namespace fumasu
{
	
class Context
{
public:
	Context() =default;
	~Context() =default;
	
	void SetGameSize(const size_t width, const size_t height)
	{
		GameWidth =width;
		GameHeight =height;
	}
	void SetGameSize(const glm::vec2 size)
	{
		SetGameSize(size.x, size.y);
	}
	
	void GetGameSize(size_t& width, size_t& height)
	{
		width =GameWidth;
		height =GameHeight;
	}
	void GetGameSize(glm::vec2& size)
	{
		size.x =GameWidth;
		size.y =GameHeight;
	}
	
	void AddText(std::string identifier, Text::Ptr text);
	Text* GetText(std::string identifier);
	
	Poco::Logger& GetLogger(const std::string name);

private:
	size_t GameWidth, GameHeight;
	std::map<std::string, Text::Ptr> Texts;
};

}

#endif // CONTEXT_HPP
