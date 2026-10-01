#pragma once
#include "vec3.hpp"
#include "gtc/quaternion.hpp"

namespace glm
{
    // transform in East-Up-South (RHS)
    struct transform
    {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
        
        transform(glm::vec3 pos, glm::quat rot) : position(pos), rotation(rot) {};
        transform(){};
        transform(const transform&) = default;

        // Asignment operator
        transform& operator=(const transform& other) {
            position = other.position;
            rotation = other.rotation;
            return *this;
        }
        
        // Construct from Matrix (Decomposition)
        explicit transform(const glm::mat4& m) {
            position = glm::vec3(m[3]);
            rotation = glm::quat_cast(m);
        }
        
        glm::mat4 toMatrix() const
        {
            glm::mat4 model = glm::mat4_cast(rotation);
            model[3] = glm::vec4(position, 1.0f);
            return model;
        }
        
        // Multiplication (Composition of Transforms)
        // Transform A * Transform B means "Apply B, then Apply A"
        transform operator*(const transform& rhs) const {
            return transform(
                (rotation * rhs.position) + position,
                rotation * rhs.rotation
            );
        }

        transform inverse() const {
            glm::quat invRot = glm::conjugate(rotation);
            return transform(
                -(invRot * position),
                invRot
            );
        }
        
        // Helper: Forward direction (-Z-axis)
        glm::vec3 forward() const { return rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
        // Helper: Right direction (X-axis)
        glm::vec3 right() const { return rotation * glm::vec3(1.0f, 0.0f, 0.0f); }
        // Helper: Up direction (Y-axis)
        glm::vec3 up() const { return rotation * glm::vec3(0.0f, 1.0f, 0.0f); }
        // Helper: Left direction (-X-axis)
        glm::vec3 left() const { return rotation * glm::vec3(-1.0f, 0.0f, 0.0f); }
        // Helper: Backward direction (Z-axis)
        glm::vec3 backward() const { return rotation * glm::vec3(0.0f, 0.0f, 1.0f); }
        // Helper: Down direction (-Y-axis)
        glm::vec3 down() const { return rotation * glm::vec3(0.0f, -1.0f, 0.0f); }
    };
}