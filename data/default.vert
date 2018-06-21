attribute vec3 vnormal;
attribute vec3 vpos;
attribute vec2 vtexture;
attribute vec3 vtangent;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 nmatrix;
uniform mat4 modelmatrix;
uniform vec3 eyePos;

uniform bool texRFLbool;
uniform bool texRFRbool;
uniform float texRFRcoef;

varying vec2 ftexture;
varying vec3 fnormal;
varying vec4 vertexObs;
varying mat3 TBN;
varying vec3 vertexCoords;


void main() {
	gl_Position = mvp * vec4(vpos, 1.0);
	
	vertexObs = mv * vec4(vpos, 1.0);
	
	vec4 fnormal0 = nmatrix * vec4(vnormal, 0);
	fnormal = fnormal0.xyz;
	
	ftexture = vtexture;

	vec3 T = (nmatrix * vec4(vtangent, 1.0)).xyz;
	vec3 B = cross(fnormal, T);
	TBN = transpose(mat3(T, B, fnormal));
		
	vec3 eyev = normalize( vec3(modelmatrix * vec4(vpos, 1)) - eyePos);
	vec3 normalv = vec3(modelmatrix * vec4(vnormal, 0));
	
	if (texRFLbool) 
		vertexCoords = reflect(eyev, normalv);
	else if (texRFRbool)
		vertexCoords = refract(eyev, normalv, texRFRcoef);
	else 
		vertexCoords = vpos;
	
	
}