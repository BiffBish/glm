#pragma once

#include "ray.hpp"

namespace glm {
	struct plane 
	{
		// Normal (x, y, z) and distance (d)
		// The equation of the plane is: dot(normal, point) + d = 0
		glm::vec4 data; 

		// Helper accessors for convenience
		inline glm::vec3 normal() const { return glm::vec3(data); }
		inline float distance() const { return data.w; }

		static plane fromPointNormal(const glm::vec3& point, const glm::vec3& normal) 
		{
			plane p;
			p.data = glm::vec4(glm::normalize(normal), -glm::dot(normal, point));
			return p;
		}
	};


	inline bool intersect(const plane& p1, const plane& p2, glm::ray& result) {
		glm::vec3 n1 = glm::vec3(p1.data);
		glm::vec3 n2 = glm::vec3(p2.data);
    
		// 1. The direction of the intersection line is the cross product of normals
		glm::vec3 dir = glm::cross(n1, n2);
		float denom = glm::dot(dir, dir);
    
		// 2. If the denominator is near 0, the planes are parallel and don't intersect
		if (denom < 1e-6f) return false;
    
		// 3. Find a point on the line
		// Using the formula: p = (d1*(n2 x dir) + d2*(dir x n1)) / |dir|^2
		float d1 = -p1.data.w;
		float d2 = -p2.data.w;
    
		glm::vec3 origin = (d1 * glm::cross(n2, dir) + d2 * glm::cross(dir, n1)) / denom;
    
		result.origin = origin;
		result.direction = glm::normalize(dir);
    
		return true;
	}
}	

