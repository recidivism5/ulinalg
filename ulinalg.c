#define _USE_MATH_DEFINES
#include <math.h>
typedef union IVec3{
	struct {int x,y,z;};
	int arr[3];
}IVec3;
typedef union FVec2{
	struct {float x,y;};
	float arr[2];
}FVec2;
typedef union FVec3{
	struct {float x,y,z;};
	float arr[3];
}FVec3;
typedef union FVec4{
	struct {FVec3 vec3; float ww;};
	struct {float x,y,z,w;};
	float arr[4];
}FVec4;
typedef union Mat4{
	struct {FVec4 a,b,c,d;};
	struct {float 
		a0,a1,a2,a3,
		b0,b1,b2,b3,
		c0,c1,c2,c3,
		d0,d1,d2,d3;};
	float arr[16];
}Mat4;
#define FOR(var,count) for(int var = 0; var < (count); var++)
float fvec3Dot(FVec3 a, FVec3 b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}
float fvec3Length(FVec3 v){
	return sqrtf(fvec3Dot(v,v));
}
FVec3 fvec3Cross(FVec3 a, FVec3 b){
	return (FVec3){
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}
FVec3 fvec3Add(FVec3 a, FVec3 b){
	FOR(i,3) a.arr[i] += b.arr[i];
	return a;
}
FVec3 fvec3Sub(FVec3 a, FVec3 b){
	FOR(i,3) a.arr[i] -= b.arr[i];
	return a;
}
FVec3 fvec3Scale(FVec3 v, float s){
	FOR(i,3) v.arr[i] *= s;
	return v;
}
FVec3 fvec3Norm(FVec3 v){
	return fvec3Scale(v, 1.0f/fvec3Length(v));
}
FVec3 fvec3SetLength(FVec3 v, float length){
	return fvec3Scale(fvec3Norm(v), length);
}
FVec3 fvec3Midpoint(FVec3 a, FVec3 b){
	return fvec3Add(fvec3Scale(fvec3Sub(b,a),0.5f),a);
}
float fvec3Dist(FVec3 a, FVec3 b){
	return fvec3Length(fvec3Sub(a,b));
}
float fvec3AngleBetween(FVec3 a, FVec3 b){
	return acosf(fvec3Dot(a,b)/(fvec3Length(a)*fvec3Length(b)));
}
FVec3 clampEuler(FVec3 e){
	float fp = 4*M_PI;
	FOR(i,3) if (e.arr[i] > fp) e.arr[i] -= fp;
		else if (e.arr[i] < -fp) e.arr[i] += fp;
	return e;
}
Mat4 mat4Identity(void){
	return (Mat4){
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}
Mat4 mat4Basis(FVec3 x, FVec3 y, FVec3 z){
	Mat4 m = mat4Identity();
	m.a.vec3 = x;
	m.b.vec3 = y;
	m.c.vec3 = z;
	return m;
}
Mat4 mat4Transpose(Mat4 m){
	float a1 = m.a1, a2 = m.a2, a3 = m.a3,
	b2 = m.b2, b3 = m.b3,
	c3 = m.c3;
	m.a1 = m.b0; m.a2 = m.c0; m.a3 = m.d0;
	m.b2 = m.c1; m.b3 = m.d1;
	m.c3 = m.d2;

	m.b0 = a1; m.c0 = a2; m.d0 = a3;
	m.c1 = b2; m.d1 = b3;
	m.d2 = c3;
	return m;
}
Mat4 mat4Ortho(float left, float right, float bottom, float top, float near, float far){
    return (Mat4){
        2/(right-left),0,0,0,
        0,2/(top-bottom),0,0,
        0,0,2/(near-far),0,
        (right+left)/(left-right),(top+bottom)/(bottom-top),(far+near)/(near-far),1
    };
}
Mat4 mat4Persp(float fovRadians, float aspectRatio, float near, float far){
	float s = 1.0f / tanf(fovRadians * 0.5f);
	float d = near - far;
	return (Mat4){
		s/aspectRatio,0,0,0,
		0,s,0,0,
		0,0,(far+near)/d,-1,
		0,0,(2*far*near)/d,0
	};
}
Mat4 mat4Scale(FVec3 v){
	Mat4 m = mat4Identity();
	m.a0 = v.x;
	m.b1 = v.y;
	m.c2 = v.z;
	return m;
}
Mat4 mat4Pos(FVec3 v){
	Mat4 m = mat4Identity();
	m.d.vec3 = v;
	return m;
}
Mat4 mat4RotX(float angle){
	float c = cos(angle), s = sin(angle);
	return (Mat4){
		1,0,0,0,
		0,c,s,0,
		0,-s,c,0,
		0,0,0,1
	};
}
Mat4 mat4RotY(float angle){
	float c = cos(angle), s = sin(angle);
	return (Mat4){
		c,0,-s,0,
		0,1,0,0,
		s,0,c,0,
		0,0,0,1
	};
}
Mat4 mat4RotZ(float angle){
	float c = cos(angle), s = sin(angle);
	return (Mat4){
		c,s,0,0,
		-s,c,0,0,
		0,0,1,0,
		0,0,0,1
	};
}
Mat4 mat4Mul(Mat4 a, Mat4 b){
	Mat4 m;
	FOR(i,4) FOR(j,4) m.arr[i*4+j] = a.arr[j]*b.arr[i*4] + a.arr[j+4]*b.arr[i*4+1] + a.arr[j+8]*b.arr[i*4+2] + a.arr[j+12]*b.arr[i*4+3];
	return m;
}
FVec4 mat4MulFVec4(Mat4 m, FVec4 v){
	FVec4 r;
	FOR(i,4) r.arr[i] = v.x*m.a.arr[i] + v.y*m.b.arr[i] + v.z*m.c.arr[i] + v.w*m.d.arr[i];
	return r;
}
Mat4 mat4LookAt(FVec3 eye, FVec3 target){
	FVec3 z = fvec3Norm(fvec3Sub(eye,target)),
		  x = fvec3Norm(fvec3Cross(z,(FVec3){0,1,0})),
		  y = fvec3Cross(x,z);
	return mat4Mul(mat4Transpose(mat4Basis(x,y,z)),mat4Pos(fvec3Scale(eye,-1)));
}
Mat4 eulerToMat4(FVec3 e){
	return mat4Mul(mat4RotZ(e.z),mat4Mul(mat4RotY(e.y),mat4RotX(e.x)));
}
FVec3 fvec3Rotated(FVec3 v, FVec3 euler){
	return mat4MulFVec4(eulerToMat4(euler),(FVec4){v.x,v.y,v.z,0.0f}).vec3;
}