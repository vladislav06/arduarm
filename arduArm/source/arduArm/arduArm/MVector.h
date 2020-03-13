#ifndef _MVECTOR_H_
#define _MVECTOR_H_
namespace MVector {


	struct point
	{
		float x;
		float y;
		float z;
	};

	point sum(point a, point b); // c = a + b

	point dif(point a, point b); // c = a - b

	point absP(point a); //a = | a |

	point toPoint(float a); //turns float into point

	float vecLng(point a, point b); // lenght of vector 

	point multVEC(float a, point b); // float * point


}
#endif