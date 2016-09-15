#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <memory>

namespace fumasu
{

template<typename Resource, typename Identifier>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	
	const Resource& Get(Identifier id) const;
	Resource& Get(Identifier id);
	void Load(Identifier id, std::string filename);

private:
	std::map<Identifier, std::unique_ptr<Resource>> mResources;
};

}

#endif // RESOURCEMANAGER_HPP
