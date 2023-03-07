ulinalg (Micro Linear Algebra) is a tiny C linear algebra library for making 3D games. It's like my other library, linalg, but smaller and doesn't have quaternions, since I haven't needed them yet. But when I do rigid body physics I will probably add quaternions to this library.

All the functions are composable, so a 3D camera can be written like this:

typedef struct Camera{
	FVec3 position;
	FVec3 euler;
}Camera;
Mat4 defaultPersp(float aspect){
    return mat4Persp(0.5f*M_PI, aspect, 0.01f, 1024.0f);
}
Mat4 getVP(Camera *c, float aspect){
	return mat4Mul(defaultPersp(aspect),mat4Mul(mat4Transpose(eulerToMat4(c->euler)),mat4Pos(fvec3Scale(c->position,-1))));
}
void cameraRotate(Camera *c, float dx, float dy, float sens){
	c->euler.y += sens * dx;
	c->euler.x += sens * dy;
	c->euler = clampEuler(c->euler);
}