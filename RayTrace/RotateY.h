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
		sinTheta = glm::sin(glm::radians(angle));
		cosTheta = glm::sin(glm::radians(angle));
		
		hasBox = pObj->BoundingBox(0, 1, boundingBox);

		glm::vec3 min(INT16_MAX, INT16_MAX, INT16_MAX);
		glm::vec3 max(INT16_MIN, INT16_MIN, INT16_MIN);
		
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					float x = i * boundingBox.GetMax().x + (1 - i) * boundingBox.GetMin().x;
					float y = j * boundingBox.GetMax().y + (1 - j) * boundingBox.GetMin().y;
					float z = k * boundingBox.GetMax().z + (1 - k) * boundingBox.GetMin().z;
					float newX = cosTheta * x + sinTheta * z;
					float newZ = -sinTheta * x + cosTheta * z;
					
					glm::vec3 tester(newX, y, newZ);
					
					for (int c = 0; c < 3; c++) {
						max[c] = glm::max<float>(max[c], tester[c]);
						min[c] = glm::min<float>(min[c], tester[c]);
					}
				}
			}
		}
		boundingBox = AABB(min, max);
	}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		glm::vec3 origin = r.GetOrigin();
		glm::vec3 dir = r.GetDir();
		origin.x = cosTheta * r.GetOrigin().x - sinTheta * r.GetOrigin().z;
		origin.z = sinTheta * r.GetOrigin().x + cosTheta * r.GetOrigin().z;
		dir.x = cosTheta * r.GetDir().x - sinTheta * r.GetDir().z;
		dir.z = sinTheta * r.GetDir().x + cosTheta * r.GetDir().z;

		Ray rotated_r(origin, dir, r.GetTime());
		if (pObj->IsHit(rotated_r, t_min, t_max, record)) {
			glm::vec3 p = record.p;
			glm::vec3 n = record.normal;
			p.x = cosTheta * record.p.x + sinTheta * record.p.z;
			p.z = -sinTheta * record.p.x + cosTheta * record.p.z;
			n.x = cosTheta * record.normal.x + sinTheta * record.normal.z;
			n.z = -sinTheta * record.normal.z + cosTheta * record.normal.z;
			record.p = p;
			record.normal = n;
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		box = boundingBox;
		return hasBox;
	}
};