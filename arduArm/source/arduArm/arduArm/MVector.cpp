#define _USE_MATH_DEFINES
#include <cmath>
namespace MVector {

	struct point
	{
		float x;
		float y;
		float z;
	};
	point sum(point a, point b)	 // c = a + b

	{
		point c;
		c.x = a.x + b.x;
		c.y = a.y + b.y;
		c.z = a.z + b.z;
		return c;
	}
	
	point dif(point a, point b) // c = a - b
	{
		point c;
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
		return c;
	}
	
	point absP(point a) //a = | a |
	{


		if (a.x < 0)
		{
			a.x *= -1;
		}

		if (a.y < 0)
		{
			a.y *= -1;
		}

		if (a.z < 0)
		{
			a.z *= -1;
		}

		return a;
	}

	point toPoint(float a)//turns float into point 
	{
		point b;
		b.x = a;
		b.y = a;
		b.z = a;
		return b;
	}
	float vecLng(point a, point b)
	{

		point c;
		c = absP(dif(a, b));

		float lng;
		lng = sqrt(pow(c.x, 2) + pow(c.y, 2) + pow(c.z, 2));

		return lng;

	};
	point multVEC(float a, point b) // float * point
	{
		b.x *= a;
		b.y *= a;
		b.z *= a;
		return b;
	};
	void nol() {}
}