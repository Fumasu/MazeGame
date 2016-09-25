#pragma once

class AABB
{
public:
	AABB () : c (), r ()
	{
	}
	AABB (const glm::vec3& center, const glm::vec3& halfwidths) : c (center), r (halfwidths)
	{
	}
	
	bool isColliding(const AABB& a)
	{
		bool x =abs(c.x - a.c.x) <=(r.x + a.r.x);
		bool y =abs(c.y - a.c.y) <=(r.y + a.r.y);
		bool z =abs(c.z - a.c.z) <=(r.z + a.r.z);
		
		return x && y && z;
	}
	
	/*bool containsPoint(glm::vec3 point)
	{
		if (point.x >=(c.x - r.x) && point.x <=(c.x + r.x) &&
			point.y >=(c.y - r.y) && point.y <=(c.y + r.y) &&
			point.z >=(c.z - r.z) && point.z <=(c.z + r.z))
			return true;
		
		return false;
	}*/

	glm::vec3 c, r;
	
	double abs(double a)
	{
		return std::fabs(a);
	}
};