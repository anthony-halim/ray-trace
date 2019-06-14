#pragma once

# include "Polygon.h"
# include <glm/trigonometric.hpp>

class RotateY : public Polygon {

private:
	Polygon* pObj;
	float sinTheta;
	float cosTheta;
	bool hasBox;
	AABB boundingBox;
public:
	RotateY(Polygon* p, float angle)
		: pObj(p) {
		float radians = glm::radians(angle);
		sinTheta = glm::sin(radians); 
		cosTheta = glm::cos(radians);
		hasBox = pObj->BoundingBox(0, 1, boundingBox);
		glm::vec3 min(INT32_MAX, INT32_MAX, INT32_MAX);
		glm::vec3 max(INT32_MIN, INT32_MIN, INT32_MIN);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					float x = i * boundingBox.GetMax().x + (1 - i) * boundingBox.GetMin().x;
					float y = j * boundingBox.GetMax().y + (1 - j) * boundingBox.GetMin().y;
					float z = k * boundingBox.GetMax().z + (1 - k) * boundingBox.GetMin().z;
					float newX = cosTheta * x + sinTheta * z;
					float newZ = -sinTheta * x + cosTheta * z;
					glm::vec3 test(newX, y, newZ);
					for (int c = 0; c < 3; c++) {
						max[c] = glm::max(test[c], max[c]);
						min[c] = glm::min(test[c], min[c]);
					}
				}
			}
		}
		boundingBox = AABB(min, max);
	}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		glm::vec3 origin = r.GetOrigin();
		glm::vec3 direction = r.GetDir();
		origin.x = cosTheta * r.GetOrigin().x - sinTheta * r.GetOrigin().z;
		origin.z = sinTheta * r.GetOrigin().x + cosTheta * r.GetOrigin().z;
		direction.x = cosTheta * r.GetDir().x - sinTheta * r.GetDir().z;
		direction.z = sinTheta * r.GetDir().x + cosTheta * r.GetDir().z;
		Ray rotatedRay(origin, direction, r.GetTime());
		if (pObj->IsHit(rotatedRay, t_min, t_max, record)) {
			glm::vec3 p = record.p;
			glm::vec3 normal = record.normal;
			p.x = cosTheta * record.p.x + sinTheta * record.p.z;
			p.z = -sinTheta * record.p.x + cosTheta * record.p.z;
			normal.x = cosTheta * record.normal.x + sinTheta * record.normal.z;
			normal.z = -sinTheta * record.normal.x + cosTheta * record.normal.z;
			record.p = p;
			record.normal = normal;
			return true;
		}
		return false;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		box = boundingBox;
		return hasBox;
	}
};