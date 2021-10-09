#version 430

precision highp float;

layout(local_size_x = 32, local_size_y = 32) in;

layout(binding = 0, rgba32f) uniform image2D tracedImage;

#define FLOAT_MAX 3.4028235e+38
#define FLOAT_MIN -3.4028235e+38
#define EPSILON 0.001
#define PI 3.1415926535898

#define NUM_BOXES 15//20
#define NUM_SPHERES 1
#define EPSILON 1E-4
#define LIGHT_INTENSITY 10.0
#define BACKGROUND_INTENSITY 2.0
#define SKY_COLOUR vec3(0.89, 0.96, 1.00)
#define LIGHT_DIRECTION vec3(0.0, 1.0, 0.0)

uniform samplerCube environment;

uniform vec3 eye;
uniform vec3 ray00;
uniform vec3 ray01;
uniform vec3 ray10;
uniform vec3 ray11;

uniform int current_spp;
uniform float blendFactor;

struct Material
{
	vec3 Albedo;
	float SpecularChance;

	vec3 Emissive;
	float SpecularRoughness;

	vec3 Absorbance;
	float RefractionChance;

	float RefractionRoughness;
	float IOR;
};

struct Box 
{
	vec3 min;
	vec3 max;

	Material material;
};

const Box boxes[NUM_BOXES] = 
{
	{vec3(-5.0, -0.1, -5.0), vec3(5.0, 0.0,  5.0), { vec3(1.0), 0.1, vec3(0.0), 0.0, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- floor
	{vec3(-5.0,  5.1, -5.0), vec3(5.0, 5.0,  5.0), { vec3(0.00, 0.45, 0.33), 1.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- top
	{vec3(-5.1,  0.0, -5.0), vec3(-5.0, 5.0,  5.0), { vec3(0.5, 0.2, 0.09), 0.2, vec3(0.0), 0.3, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- left wall
	{vec3(5.0,  0.0, -5.0), vec3(5.1, 5.0,  5.0), { vec3(0.4, 0.4, 0.4), 0.0, vec3(0.0), 1.0, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- right wall
	//
	//{vec3( 5.0,  0.0,  5.0), vec3( 5.1, 5.0,  3.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - left
	//{vec3( 5.0,  0.0,  1.5), vec3( 5.1, 5.0, -2.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - middle
	//{vec3( 5.0,  3.0,  3.0), vec3( 5.1, 5.0,  1.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - door top
	//{vec3( 5.0,  0.0, -3.5), vec3( 5.1, 5.0, -5.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - right
	//{vec3( 5.0,  0.0, -2.0), vec3( 5.1, 2.5, -3.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - top
	//{vec3( 5.0,  3.8, -2.0), vec3( 5.1, 5.0, -3.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - bottom
	//
	{vec3(-5.0,  0.0, -5.1), vec3(5.0, 5.0, -5.0), { vec3(0.43, 0.52, 0.27), 0.0, vec3(0.0), 1.0, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- back wall
	{vec3(-5.0,  0.0,  5.0), vec3(5.0, 5.0,  5.1), { vec3(0.5, 0.2, 0.09), 0.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- front wall
	{vec3(-1.0,  1.0, -1.0), vec3(1.0, 1.1,  1.0), { vec3(0.3, 0.23, 0.15), 0.1, vec3(0.0), 0.6, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- table top
	{vec3(-1.0,  0.0, -1.0), vec3(-0.8, 1.0, -0.8), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- table foot
	{vec3(-1.0,  0.0,  0.8), vec3(-0.8, 1.0,  1.0), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 0.0 }0}, // <- table foot
	{vec3(0.8,  0.0, -1.0), vec3(1.0, 1.0, -0.8), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- table foot
	{vec3(0.8,  0.0,  0.8), vec3(1.0, 1.0,  1.0), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 0.0 }}, // <- table foot
	{vec3(3.0,  0.0, -4.9), vec3(3.3, 2.0, -4.6), { vec3(0.6, 0.6, 0.6), 1.0, vec3(0.0), 0.1, vec3(0.0), 0.0, 0.0, 0.0 }},  // <- some "pillar"
	//{vec3(-4.8,  4.9, -4.7), vec3( 4.8, 5.0, -4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(5.0, 0, 0)},  // <- some "LED"
	//{vec3(-4.8,  4.9,  4.7), vec3( 4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(0, 5.0, 0)},  // <- some "LED"
	//{vec3(-4.7,  4.9, -4.8), vec3(-4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(0, 0, 5.0)},  // <- some "LED"
	//{vec3( 4.7,  4.9, -4.8), vec3( 4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(5.0, 5.0, 0)},  // <- some "LED"
	{vec3(-2.0,  2.5, -4.9), vec3(2.0, 3.0, -5.0), { vec3(1.0, 1.0, 1.0), 1.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 0.0 }},  // <- Big LED
	{vec3(9.0,  2.5, -0.5), vec3(10.0, 3.5, 0.5), { vec3(1.0, 1.0, 1.0), 1.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 0.0 }},  // <- Big glass
	{vec3(-0.2, 1.1, -0.01), vec3(0.2, 2.0, 0.01), { vec3(0.56, 0.57, 0.58), 1.0, vec3(0.0), 0.05, vec3(0.0), 0.0, 0.0, 0.0 }},  // <- Big mirror
};

struct HitInfo 
{
	bool fromInside;
	vec3 nearHitPos;
	vec3 normal;

	Material Material;
};

struct Ray
{
	vec3 origin;
	vec3 direction;
};

vec3 Radiance(Ray ray);
bool GetClosestIntersectingRayObject(Ray ray, out HitInfo hitInfo);
bool RayCubeIntersect(Ray ray, vec3 aabbMin, vec3 aabbMax, out float t1, out float t2);
vec3 GetNormal(vec3 spherePos, float radius, vec3 surfacePosition);
vec3 GetCosWeightedHemissphereDir(inout uint rndSeed, vec3 normal);
vec2 GetPointOnCircle(inout uint rndSeed);
uint GetPCGHash(inout uint seed);
float GetRandomFloat01(inout uint state);
float GetSmallestPositive(float t1, float t2);
float FresnelSchlick(float cosTheta, float n1, float n2);

uniform int rayDepth = 3;
uniform int SSP = 1;

uniform float focalLength = 200.0;
uniform float apertureDiameter = 0.5;

uint rndSeed;

void main(void) 
{
	pixel = ivec2(gl_GlobalInvocationID.xy);
	ivec2 size = imageSize(tracedImage);

	time = time_;
	_current_spp = current_spp_ * SAMPLES_PER_INVOCATION;

	if (any(greaterThanEqual(pixel, size)))
		return;
		
	vec2 point = (vec2(pixel) + vec2(0.5)) / vec2(size);
	vec3 dir = mix(mix(ray00, ray01, point.y), mix(ray10, ray11, point.y), point.x);
	vec3 norm_dir = normalize(dir);

	if (MULTI_SAMPLE_AT_ONCE != 0)
	{
		vec3 oldColour = vec3(0.0);
		if (current_spp_ >= 2)
		{
			oldColour = imageLoad(tracedImage, pixel).rgb;
		}

		vec3 final = oldColour;

		for (int i = 0; i < SAMPLES_PER_INVOCATION; i++)
		{
			_current_spp++;
			vec3 colour = Trace(eye, norm_dir);
			final = mix(colour, final, (_current_spp) / (_current_spp + 1.0));
			time += 0.01;
		}

		imageStore(tracedImage, pixel, vec4(final, 1.0));
	}
	else
	{
		vec3 colour = Trace(eye, normalize(dir));
		vec3 oldColour;

		if (blendFactor > 0.0)
		{
			oldColour = imageLoad(tracedImage, pixel).rgb;
		}

		vec3 finalColour = mix(colour, oldColour, blendFactor);
		imageStore(tracedImage, pixel, vec4(finalColour, 1.0));
	}
}
vec3 Trace(vec3 origin, vec3 direction)
{
	HitInfo info;
	Ray ray = { origin, direction };

	vec3 attenuation = vec3(1.0);
	for (int i = 0; i < MAX_BOUNCES; i++)
	{
		if (!IntersectObjects(ray, info))
		{
			//return SKY_COLOUR * LIGHT_INTENSITY * attenuation * max(dot(ray.direction, LIGHT_DIRECTION), 0.0);
			vec3 env = texture(environment, normalize(ray.direction)).rgb;
			//env = clamp(env, vec3(0.0), vec3(1000.0));
			return env * attenuation;
		}

		vec3 rand = randvec3(i);

		Box box = boxes[info.index];
		vec3 normal = info.normal;
		vec3 albedo = box.albedo;
		float roughness = box.roughness;
		float metallic = box.metallic;
		vec3 emission = box.emission;
		float specTrans = box.specTrans;

		if (emission.r != 0 || emission.g != 0 || emission.b != 0)
			return attenuation * emission;

		vec4 samplePDF = randomHemispherePoint(normal, rand.xy);
		vec3 point = ray.origin + info.near * ray.direction;

		ray.origin = point + normal * EPSILON;

		/*attenuation *= albedo;
		ray.direction = samplePDF.xyz;
		attenuation *= max(dot(ray.direction, normal), 0.0);*/

		vec3 F0 = vec3(0.04);
		F0 = mix(F0, albedo, metallic);
		vec3 F = FresnelSchlickRoughness(max(dot(normal, -ray.direction), 0), F0, roughness);

		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;

		vec3 _brdf = brdf(albedo, samplePDF.xyz, -ray.direction, normal, kD, F0, roughness, metallic);

		if (is_nan(_brdf))
		{
			return vec3(0, 0, 0);
		}

		attenuation *= brdf(albedo, samplePDF.xyz, -ray.direction, normal, kD, F0, roughness, metallic);

		ray.direction = samplePDF.xyz;
		float ndotv = max(dot(ray.direction, normal), 0.0);
		if (isnan(ndotv))
		{
			return vec3(1, 1, 1);
		}
		attenuation *= ndotv;

		//if (samplePDF.w > 0.0)
		{
			attenuation /= max(samplePDF.w, 0.0001);
		}
	}

	return vec3(0.0);
}


vec2 IntersectBox(Ray ray, const Box box, out vec3 normal)
{
	vec3 tMin = (box.min - ray.origin) / ray.direction;
	vec3 tMax = (box.max - ray.origin) / ray.direction;

	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);

	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	normal = vec3(equal(t1, vec3(tNear))) * sign(-ray.direction);

	return vec2(tNear, tFar);
}

bool IntersectObjects(Ray ray, out HitInfo info)
{
	float smallest = LARGE_FLOAT;
	bool found = false;
	vec3 normal;

	for (int i = 0; i < NUM_BOXES; i++)
	{
		vec2 lambda = IntersectBox(ray, boxes[i], normal);
		if (lambda.y >= 0.0 && lambda.x < lambda.y && lambda.x < smallest)
		{
			info.near = lambda.x;
			info.index = i;
			info.normal = normal;
			smallest = lambda.x;
			found = true;
		}
	}

	return found;
}

vec3 randvec3(int seed)
{
	return random3(vec3(pixel + ivec2(seed), time));
}

vec3 brdfSpecular(vec3 lightInDir, vec3 lightOutDir, vec3 normal, vec3 F0, float roughness, float metallic)
{
	//normal -> N, lightInDir -> L, lightOutDir -> V
	vec3 H = normalize(lightInDir + lightOutDir);

	float NDF = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, lightOutDir, lightInDir, roughness);
	vec3 F = FresnelSchlick(max(dot(H, lightOutDir), 0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1) - kS;
	kD *= 1 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4 * max(dot(normal, lightOutDir), 0) * max(dot(normal, lightInDir), 0);
	vec3 specular = numerator / max(denominator, 0.001);

	return specular;
}

vec3 brdfDiffuse(vec3 albedo, vec3 lightInDir, vec3 lightOutDir, vec3 normal)
{
	return albedo * ONE_OVER_PI;
}

vec3 brdf(vec3 albedo, vec3 lightInDir, vec3 lightOutDir, vec3 normal, vec3 kD, vec3 F0, float roughness, float metallic)
{
	return brdfSpecular(lightInDir, lightOutDir, normal, F0, roughness, metallic)
		+ brdfDiffuse(albedo, lightInDir, lightOutDir, normal) * kD;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1 - F0) * pow(1.0 - cosTheta, 5.0);
}

#define spatialrand vec2

vec3 ortho(vec3 v)
{
	return normalize(abs(v.x) > abs(v.z) ? vec3(-v.y, v.x, 0.0) : vec3(0.0, -v.z, v.y));
}

uvec3 pcg3d(uvec3 v)
{
	v = v * 1664525u + 1013904223u;

	v.x += v.y * v.z;
	v.y += v.z * v.x;
	v.z += v.x * v.y;

	v ^= v >> 16u;

	v.x += v.y * v.z;
	v.y += v.z * v.x;
	v.z += v.x * v.y;

	return v;
}

vec3 random3(vec3 f)
{
	return uintBitsToFloat((pcg3d(floatBitsToUint(f)) & 0x007FFFFFu) | 0x3F800000u) - 1.0;
}

vec3 around(vec3 v, vec3 z)
{
	vec3 t = ortho(z), b = cross(z, t);
	return t * v.x + b * v.y + z * v.z;
}

vec3 isotropic(float rp, float c)
{
	float p = TWO_PI * rp, s = sqrt(1.0 - c * c);
	return vec3(cos(p) * s, sin(p) * s, c);
}

vec4 randomHemispherePoint(vec3 n, spatialrand rand)
{
	return vec4(around(isotropic(rand.x, rand.y), n), ONE_OVER_2PI);
}

float hemisphereProbability(vec3 n, vec3 v)
{
	return step(0.0, dot(v, n)) * ONE_OVER_2PI;
}

vec4 randomDiskPoint(vec3 n, float d, float r, spatialrand rand)
{
	float D = r / d, c = inversesqrt(1.0 + D * D), pr = ONE_OVER_2PI / (1.0 - c);
	return vec4(around(isotropic(rand.x, 1.0 - rand.y * (1.0 - c)), n), pr);
}

float diskProbability(vec3 n, float d, float r, vec3 v)
{
	float D = r / d, c = inversesqrt(1.0 + D * D);
	return step(c, dot(n, v)) * ONE_OVER_2PI / (1.0 - c);
}