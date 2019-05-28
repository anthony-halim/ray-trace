#pragma once

#include "Polygon.h"


class Rectangle_XY : public Polygon {
private:
	Material* pMat;
	float x0, x1, y0, y1, k;
public:
	Rectangle_XY()
		: pMat(nullptr), x0(0.0f), x1(0.0f), y0(0.0f), y1(0.0f), k(0.0f) {}

	Rectangle_XY(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), pMat(mat) {}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		float t = (k - r.GetOrigin().z) / r.GetDir().z;
		
		if (t < t_min || t > t_max) return false;

		float x = r.GetOrigin().x + t * r.GetDir().x;
		float y = r.GetOrigin().y + t * r.GetDir().y;

		if (x < x0 || x > x1 || y < y0 || y > y1) return false;

		record.u = (x - x0) / (x1 - x0);
		record.v = (y - y0) / (y1 - y0);
		record.t = t;
		record.pMat_ptr = pMat;
		record.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		
		return true;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		box = AABB(glm::vec3(x0, y0, k - 0.0001f), glm::vec3(x1, y1, k + 0.0001f));
		return true;
	}
};



class Rectangle_XZ : public Polygon {
private:
	Material* pMat;
	float x0, x1, z0, z1, k;
public:
	Rectangle_XZ()
		: pMat(nullptr), x0(0.0f), x1(0.0f), z0(0.0f), z1(0.0f), k(0.0f) {}

	Rectangle_XZ(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), pMat(mat) {}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		float t = (k - r.GetOrigin().y) / r.GetDir().y;

		if (t < t_min || t > t_max) return false;

		float x = r.GetOrigin().x + t * r.GetDir().x;
		float z = r.GetOrigin().z + t * r.GetDir().z;

		if (x < x0 || x > x1 || z < z0 || z > z1) return false;

		record.u = (x - x0) / (x1 - x0);
		record.v = (z - z0) / (z1 - z0);
		record.t = t;
		record.pMat_ptr = pMat;
		record.normal = glm::vec3(0.0f, 1.0f, 0.0f);

		return true;
	}

	virtual bool BoundingBox(float t0, float t1, AABB & box) const override {
		box = AABB(glm::vec3(x0, k - 0.0001f, z0), glm::vec3(x1, k + 0.0001f, z1));
		return true;
	}
};



class Rectangle_YZ : public Polygon {
private:
	Material* pMat;
	float y0, y1, z0, z1, k;
public:
	Rectangle_YZ()
		: pMat(nullptr), y0(0.0f), y1(0.0f), z0(0.0f), z1(0.0f), k(0.0f) {}

	Rectangle_YZ(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), pMat(mat) {}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		float t = (k - r.GetOrigin().x) / r.GetDir().x;

		if (t < t_min || t > t_max) return false;

		float y = r.GetOrigin().y + t * r.GetDir().y;
		float z = r.GetOrigin().z + t * r.GetDir().z;

		if (y < y0 || y > y1 || z < z0 || z > z1) return false;

		record.u = (y - y0) / (y1 - y0);
		record.v = (z - z0) / (z1 - z0);
		record.t = t;
		record.pMat_ptr = pMat;
		record.normal = glm::vec3(0.0f, 0.0f, 1.0f);

		return true;
	}

	virtual bool BoundingBox(float t0, float t1, AABB & box) const override {
		box = AABB(glm::vec3(k - 0.0001f, y0, z0), glm::vec3(k + 0.0001f, y1, z1));
		return true;
	}
};