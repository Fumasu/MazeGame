#include "Context.hpp"

namespace fumasu
{


void Context::AddText(std::string identifier, Text::Ptr text)
{
	auto found =Texts.find(identifier);
	if (found !=Texts.end())		//Text already exists? do nothing
		return;
	
	Texts.emplace(identifier, std::move(text));
}

Text* Context::GetText(std::string identifier)
{
	auto found =Texts.find(identifier);
	assert (found !=Texts.end());
	
	return Texts[identifier].get();
}

Poco::Logger& Context::GetLogger(const std::string name)
{
	return Poco::Logger::get(name);
}


}

