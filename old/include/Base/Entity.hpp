#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
public:
	virtual Entity() =delete;
	virtual ~Entity() =default;
	
	virtual void Update(sf::Time dt) =0;
	virtual void Draw() =0;
	
protected:
	
private:
	
};

#endif