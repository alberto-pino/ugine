
varying vec2 ftexture;
varying vec3 fnormal;
varying vec4 vertexObs;
varying mat3 TBN;
varying vec3 vertexCoords;

uniform bool texbool;
uniform sampler2D texsampler;
uniform bool texRFLbool;
uniform sampler2D texRFLsampler;
uniform bool texRFRbool;
uniform sampler2D texRFRsampler;
uniform bool texNMbool;
uniform sampler2D texNMsampler;

uniform bool cubebool;
uniform samplerCube cubesampler;
uniform samplerCube cubeRFLsampler;
uniform samplerCube cubeRFRsampler;
uniform samplerCube cubeNMsampler;

uniform int nLights;
uniform vec3 ambient;
uniform int shininess;
uniform vec4 color;
//uniform float texRFRcoef;


vec4 difuse;
vec4 specular;

const int MAX = 10;

struct LightShader                                                           
{  
	vec4 position;
	vec4 rotation;
	vec4 lightColor;
	float linearAttenuation;
};
uniform LightShader lights[MAX];

void main() {
	if (nLights>0)
		difuse = vec4(ambient,1.0);
	else 
		difuse = vec4(1.0,1.0,1.0,1.0);
	
	specular = vec4(0,0,0,1);
	
	vec3 uvw = normalize(vertexCoords);



	for (int i=0; i<nLights; i++)
	{
		
		vec3 N=normalize(fnormal);
		
		if (texNMbool) 
		{
			vec3 rgbToTranslate = texture(texNMsampler, ftexture).rgb;
			N = normalize(normalize(rgbToTranslate * 2.0 - 1.0) * TBN);
		}
		
		vec3 L=lights[i].position.xyz;

		float attenuation = 1.0;

		if (lights[i].position.w == 1.0)
		{
			L = L.xyz-vertexObs.xyz;
			attenuation = 1.0 / (1.0 + (lights[i].linearAttenuation * length(L)));
		}
		else 
		{
			L = lights[i].rotation.xyz;
		}

		L = normalize(L);
		float NescL = max(dot(N, L), 0.0);

		difuse += NescL * attenuation * lights[i].lightColor;

		if ((shininess > 0) && (NescL > 0.0))
		{
			vec4 vertexObsN = normalize(vertexObs);
			vec3 H = vec3(L.x-vertexObsN.x, L.y-vertexObsN.y, L.z-vertexObsN.z);
			H = normalize(H);

			float NescH = max(dot(N, H), 0.0);

			specular += pow(NescH, float(shininess)) * attenuation;
		}
	}	
	
	//vec3 finalcolor = vec3(1.0, 1.0, 1.0);
	
	vec4 lightcolor = vec4(1.0, 1.0, 1.0, 1.0);
	if (texbool) 
	{
		if(cubebool) 
			lightcolor = lightcolor * textureCube(cubesampler, uvw);
		else 
			lightcolor = lightcolor * texture2D(texsampler, ftexture);	
	}

	lightcolor = (lightcolor * color * difuse) + specular;

	vec4 reflectionColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 refractionColor = vec4(0.0, 0.0, 0.0, 0.0);

	if (texRFLbool)
		reflectionColor = textureCube(cubeRFLsampler, uvw);

	if (texRFRbool) 
		refractionColor = textureCube(cubeRFRsampler, uvw);
	
	vec3 finalcolor = mix(lightcolor.rgb, reflectionColor.rgb, reflectionColor.a);
	finalcolor = mix(finalcolor.rgb, refractionColor.rgb, refractionColor.a);

	gl_FragColor = vec4(finalcolor, 1.0f);

}
