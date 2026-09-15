#pragma once

namespace glm 
{
	// Represents a sheet in 3d space. With an origin, normal, right, width, height
	struct sheet {
		glm::vec3 center;
		glm::vec3 normal;
		glm::vec3 right;
		
		float width; // The width along the X axis; From edge to edge
		float height; // The height along the Y axis; From edge to edge
	};
	
	inline bool get_sheet_intersection_segment(const ray& ray, const sheet& sheet, glm::vec2& start, glm::vec2& end) {
		float hw = sheet.width / 2.0f;
		float hh = sheet.height / 2.0f;
    
		// Project ray into local space
		glm::vec3 rel_origin = ray.origin - sheet.center;
		glm::vec3 up = glm::cross(sheet.normal, sheet.right);
		float ox = glm::dot(rel_origin, sheet.right);
		float oy = glm::dot(rel_origin, up);
		float vx = glm::dot(ray.direction, sheet.right);
		float vy = glm::dot(ray.direction, up);
    
		std::vector<float> t_values;

		// Helper to check and add valid t
		auto check_t = [&](float t, float x, float y) {
			if (abs(x) <= hw + 1e-4f && abs(y) <= hh + 1e-4f) {
				t_values.push_back(t);
			}
		};

		// Test X boundaries
		if (abs(vx) > 1e-6f) {
			float tx1 = (hw - ox) / vx;
			check_t(tx1, hw, oy + tx1 * vy);
			float tx2 = (-hw - ox) / vx;
			check_t(tx2, -hw, oy + tx2 * vy);
		}
		// Test Y boundaries
		if (abs(vy) > 1e-6f) {
			float ty1 = (hh - oy) / vy;
			check_t(ty1, ox + ty1 * vx, hh);
			float ty2 = (-hh - oy) / vy;
			check_t(ty2, ox + ty2 * vx, -hh);
		}

		if (t_values.size() < 2) return false;

		// Sort and take min/max
		float t_min = *std::min_element(t_values.begin(), t_values.end());
		float t_max = *std::max_element(t_values.begin(), t_values.end());

		start = glm::vec2(ox + t_min * vx, oy + t_min * vy);
		end   = glm::vec2(ox + t_max * vx, oy + t_max * vy);
		return true;
	}
	
};
