#version 430

precision highp float;

layout(local_size_x = 32, local_size_y = 32) in;

layout(binding = 0, rgba32f) uniform image2D tracedImage;

#define FLOAT_MAX 3.4028235e+38
#define FLOAT_MIN -3.4028235e+38
#define PI 3.1415926535898

#define NUM_BOXES 15//20
#define NUM_SPHERES 1
#define EPSILON 1E-4
#define LIGHT_INTENSITY 10.0
#define BACKGROUND_INTENSITY 2.0
#define SKY_COLOUR vec3(0.89, 0.96, 1.00)
#define LIGHT_DIRECTION vec3(0.0, 1.0, 0.0)

uniform samplerCube environment; 

uniform mat4 Projection;
uniform mat4 InvProjection;
uniform vec2 NearFar;
uniform mat4 View;
uniform mat4 InvView;
uniform mat4 ProjectionView;
uniform vec3 ViewPos;
uniform vec3 ViewDir;

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
	vec3 Min;
	vec3 Max;

	Material Material;
};

const Box boxes[NUM_BOXES] = 
{
	{vec3(-5.0, -0.1, -5.0), vec3(5.0, 0.0,  5.0), { vec3(1.0), 0.1, vec3(0.0), 0.0, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- floor
	{vec3(-5.0,  5.1, -5.0), vec3(5.0, 5.0,  5.0), { vec3(0.00, 0.45, 0.33), 0.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- top
	{vec3(-5.1,  0.0, -5.0), vec3(-5.0, 5.0,  5.0), { vec3(0.5, 0.2, 0.09), 0.2, vec3(0.0), 0.3, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- left wall
	{vec3(5.0,  0.0, -5.0), vec3(5.1, 5.0,  5.0), { vec3(0.4, 0.4, 0.4), 0.2, vec3(0.0), 0.2, vec3(0.0), 0.8, 0.2, 1.1 }}, // <- right wall
	//
	//{vec3( 5.0,  0.0,  5.0), vec3( 5.1, 5.0,  3.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - left
	//{vec3( 5.0,  0.0,  1.5), vec3( 5.1, 5.0, -2.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - middle
	//{vec3( 5.0,  3.0,  3.0), vec3( 5.1, 5.0,  1.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - door top
	//{vec3( 5.0,  0.0, -3.5), vec3( 5.1, 5.0, -5.0), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - right
	//{vec3( 5.0,  0.0, -2.0), vec3( 5.1, 2.5, -3.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - top
	//{vec3( 5.0,  3.8, -2.0), vec3( 5.1, 5.0, -3.5), vec3(0.4, 0.4, 0.4), 0.3, 0.6, vec3(0.0), 0.0}, // <- right wall - bottom
	//
	{vec3(-5.0,  0.0, -5.1), vec3(5.0, 5.0, -5.0), { vec3(0.43, 0.52, 0.27), 0.2, vec3(0.0), 1.0, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- back wall
	{vec3(-5.0,  0.0,  5.0), vec3(5.0, 5.0,  5.1), { vec3(0.5, 0.2, 0.09), 0.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- front wall
	{vec3(-1.0,  1.0, -1.0), vec3(1.0, 1.1,  1.0), { vec3(0.3, 0.23, 0.15), 0.1, vec3(0.0), 0.6, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- table top
	{vec3(-1.0,  0.0, -1.0), vec3(-0.8, 1.0, -0.8), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- table foot
	{vec3(-1.0,  0.0,  0.8), vec3(-0.8, 1.0,  1.0), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- table foot
	{vec3(0.8,  0.0, -1.0), vec3(1.0, 1.0, -0.8), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- table foot
	{vec3(0.8,  0.0,  0.8), vec3(1.0, 1.0,  1.0), { vec3(0.4, 0.3, 0.15), 0.1, vec3(0.0), 0.4, vec3(0.0), 0.0, 0.0, 1.0 }}, // <- table foot
	{vec3(3.0,  0.0, -4.9), vec3(3.3, 2.0, -4.6), { vec3(0.6, 0.6, 0.6), 1.0, vec3(0.0), 0.1, vec3(0.0), 0.0, 0.0, 1.0 }},  // <- some "pillar"
	//{vec3(-4.8,  4.9, -4.7), vec3( 4.8, 5.0, -4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(5.0, 0, 0)},  // <- some "LED"
	//{vec3(-4.8,  4.9,  4.7), vec3( 4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(0, 5.0, 0)},  // <- some "LED"
	//{vec3(-4.7,  4.9, -4.8), vec3(-4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(0, 0, 5.0)},  // <- some "LED"
	//{vec3( 4.7,  4.9, -4.8), vec3( 4.8, 5.0,  4.8), vec3(1.0, 1.0, 1.0), 1.0, 0.2, vec3(5.0, 5.0, 0)},  // <- some "LED"
	{vec3(-2.0,  2.5, -4.9), vec3(2.0, 3.0, -5.0), { vec3(1.0, 1.0, 1.0), 1.0, vec3(10.0), 0.2, vec3(0.0), 0.0, 0.0, 1.0 }},  // <- Big LED
	{vec3(9.0,  2.5, -0.5), vec3(10.0, 3.5, 0.5), { vec3(1.0, 1.0, 1.0), 1.0, vec3(0.0), 0.2, vec3(0.0), 0.0, 0.0, 1.0 }},  // <- Big glass
	{vec3(-0.2, 1.1, -0.01), vec3(0.2, 2.0, 0.01), { vec3(0.56, 0.57, 0.58), 1.0, vec3(0.0), 0.05, vec3(0.0), 0.0, 0.0, 1.0 }},  // <- Big mirror
};

struct HitInfo 
{
    float T;
	bool FromInside;
	vec3 NearHitPos;
	vec3 Normal;

	Material Material;
};

struct Ray
{
	vec3 Origin;
	vec3 Direction;
};

vec3 Radiance(Ray ray);
bool GetClosestIntersectingRayObject(Ray ray, out HitInfo hitInfo);
bool RayCuboidIntersect(Ray ray, vec3 aabbMin, vec3 aabbMax, out float t1, out float t2);
vec3 GetNormal(vec3 spherePos, float radius, vec3 surfacePosition);
vec3 GetNormal(vec3 aabbMin, vec3 aabbMax, vec3 surfacePosition);
vec3 GetCosWeightedHemissphereDir(inout uint rndSeed, vec3 normal);
vec2 GetPointOnCircle(inout uint rndSeed);
uint GetPCGHash(inout uint seed);
float GetRandomFloat01(inout uint state);
float GetSmallestPositive(float t1, float t2);
float FresnelSchlick(float cosTheta, float n1, float n2);
Ray GetWorldSpaceRay(mat4 inverseProj, mat4 inverseView, vec3 viewPos, vec2 normalizedDeviceCoords);

uniform int rayDepth = 15;
uniform int SSP = 50;

uniform float focalLength = 5.0;
uniform float apertureDiameter = 0.14;

uint rndSeed;
void main(void) 
{
	ivec2 imgResultSize = imageSize(tracedImage);
	ivec2 imgCoord = ivec2(gl_GlobalInvocationID.xy);

	rndSeed = gl_GlobalInvocationID.x * 1973 + gl_GlobalInvocationID.y * 9277 + current_spp * 2699 | 1;
	vec3 colour = vec3(0);
	for (int i = 0; i < SSP; i++)
	{
		vec2 subPixelOffset = vec2(GetRandomFloat01(rndSeed), GetRandomFloat01(rndSeed)) - 0.5; // integrating over whole pixel eliminates aliasing
		vec2 ndc = (imgCoord + subPixelOffset) / imgResultSize * 2.0 - 1.0;
		Ray rayEyeToWorld = GetWorldSpaceRay(InvProjection, InvView, ViewPos, ndc);

		vec3 focalPoint = rayEyeToWorld.Origin + rayEyeToWorld.Direction * focalLength;
		vec2 offset = apertureDiameter * 0.5 * GetPointOnCircle(rndSeed);

		rayEyeToWorld.Origin = (InvView * vec4(offset, 0.0, 1.0)).xyz;
		rayEyeToWorld.Direction = normalize(focalPoint - rayEyeToWorld.Origin);

        colour += Radiance(rayEyeToWorld);
	}

	colour /= SSP;

    if (current_spp > 1)
    {
        vec3 lastFrameColour = imageLoad(tracedImage, imgCoord).rgb;
        colour = mix(lastFrameColour, colour, 1.0 / (current_spp + 1));
    }

	imageStore(tracedImage, imgCoord, vec4(colour, 1.0));
}

vec3 Radiance(Ray ray)
{
    vec3 throughPut = vec3(1);
    vec3 ret = vec3(0);
    HitInfo hitInfo;
    for (int i = 0; i < rayDepth; i++)
    {
        if (GetClosestIntersectingRayObject(ray, hitInfo))
        {
            if (hitInfo.FromInside)
            {
                hitInfo.Normal *= -1.0;
                throughPut *= exp(-hitInfo.Material.Absorbance * hitInfo.T);
            }

            float specularChance = hitInfo.Material.SpecularChance;
            float refractionChance = hitInfo.Material.RefractionChance;
            if (specularChance > 0.0)
            {
                specularChance = mix(specularChance, 1.0, FresnelSchlick(-dot(ray.Direction, hitInfo.Normal), hitInfo.FromInside ? hitInfo.Material.IOR : 1.0, !hitInfo.FromInside ? hitInfo.Material.IOR : 1.0));
                float diffuseChance = (1.0 - (specularChance + refractionChance));
                refractionChance = (1.0 - (diffuseChance + specularChance));
            }

            float rayProbability = 1.0;
            float doSpecular = 0.0;
            float doRefraction = 0.0;
            float raySelectRoll = GetRandomFloat01(rndSeed);

            if (specularChance > raySelectRoll)
            {
                doSpecular = 1.0;
                rayProbability = specularChance;
            }
            else if (specularChance + refractionChance > raySelectRoll)
            {
                doRefraction = 1.0;
                rayProbability = refractionChance;
            }
            else
            {
                rayProbability = 1.0 - (specularChance + refractionChance);
            }

            vec3 diffuseRayDir = GetCosWeightedHemissphereDir(rndSeed, hitInfo.Normal);
            vec3 specularRayDir = normalize(mix(reflect(ray.Direction, hitInfo.Normal), diffuseRayDir, hitInfo.Material.SpecularRoughness * hitInfo.Material.SpecularRoughness));
            vec3 refractionRayDir = refract(ray.Direction, hitInfo.Normal, hitInfo.FromInside ? hitInfo.Material.IOR / 1.0 : 1.0 / hitInfo.Material.IOR);
            refractionRayDir = normalize(mix(refractionRayDir, GetCosWeightedHemissphereDir(rndSeed, -hitInfo.Normal), hitInfo.Material.RefractionRoughness * hitInfo.Material.RefractionRoughness));

            ray.Origin = hitInfo.NearHitPos + hitInfo.Normal * EPSILON * (doRefraction == 1.0 ? -1 : 1);
            ray.Direction = mix(diffuseRayDir, specularRayDir, doSpecular);
            ray.Direction = mix(ray.Direction, refractionRayDir, doRefraction);

            ret += hitInfo.Material.Emissive * throughPut;
            if (doRefraction == 0.0)
                throughPut *= hitInfo.Material.Albedo;

            rayProbability = max(rayProbability, EPSILON);
            throughPut /= rayProbability;


            float p = max(throughPut.x, max(throughPut.y, throughPut.z));
            if (GetRandomFloat01(rndSeed) > p)
                break;

            throughPut *= 1.0 / p;
        }
        else
        {
            ret += texture(environment, ray.Direction).rgb * throughPut;
            break;
        }
    }

    return ret;
}

bool GetClosestIntersectingRayObject(Ray ray, out HitInfo hitInfo)
{
    hitInfo.T = FLOAT_MAX;
    float t1, t2;

    /*for (int i = 0; i < NUM_SPHERES; i++)
    {
        vec3 pos = gameObjectsUBO.Spheres[i].Position;
        float radius = gameObjectsUBO.Spheres[i].Radius;
        if (RaySphereIntersect(ray, pos, radius, t1, t2) && t2 > 0 && t1 < hitInfo.T)
        {
            hitInfo.T = GetSmallestPositive(t1, t2);
            hitInfo.FromInside = hitInfo.T == t2;
            hitInfo.Material = gameObjectsUBO.Spheres[i].Material;
            hitInfo.NearHitPos = ray.Origin + ray.Direction * hitInfo.T;
            hitInfo.Normal = GetNormal(pos, radius, hitInfo.NearHitPos);
        }
    }*/

    for (int i = 0; i < NUM_BOXES; i++)
    {
        vec3 aabbMin = boxes[i].Min;
        vec3 aabbMax = boxes[i].Max;
        if (RayCuboidIntersect(ray, aabbMin, aabbMax, t1, t2) && t2 > 0 && t1 < hitInfo.T)
        {
            hitInfo.T = GetSmallestPositive(t1, t2);
            hitInfo.FromInside = hitInfo.T == t2;
            hitInfo.Material = boxes[i].Material;
            hitInfo.NearHitPos = ray.Origin + ray.Direction * hitInfo.T;
            hitInfo.Normal = GetNormal(aabbMin, aabbMax, hitInfo.NearHitPos);
        }
    }

    return hitInfo.T != FLOAT_MAX;
}

bool RaySphereIntersect(Ray ray, vec3 position, float radius, out float t1, out float t2)
{
    t1 = t2 = FLOAT_MAX;

    vec3 sphereToRay = ray.Origin - position;
    float b = dot(ray.Direction, sphereToRay);
    float c = dot(sphereToRay, sphereToRay) - radius * radius;
    float discriminant = b * b - c;

    if (discriminant < 0)
        return false;

    float squareRoot = sqrt(discriminant);
    t1 = -b - squareRoot;
    t2 = -b + squareRoot;

    return true;
}

bool RayCuboidIntersect(Ray ray, vec3 aabbMin, vec3 aabbMax, out float t1, out float t2)
{
    t1 = FLOAT_MIN;
    t2 = FLOAT_MAX;

    vec3 t0s = (aabbMin - ray.Origin) * (1.0 / ray.Direction);
    vec3 t1s = (aabbMax - ray.Origin) * (1.0 / ray.Direction);

    vec3 tsmaller = min(t0s, t1s);
    vec3 tbigger = max(t0s, t1s);

    t1 = max(t1, max(tsmaller.x, max(tsmaller.y, tsmaller.z)));
    t2 = min(t2, min(tbigger.x, min(tbigger.y, tbigger.z)));

    return t1 <= t2;
}

vec3 GetNormal(vec3 spherePos, float radius, vec3 surfacePosition)
{
    return (surfacePosition - spherePos) / radius;
}

vec3 GetNormal(vec3 aabbMin, vec3 aabbMax, vec3 surfacePosition)
{
    vec3 halfSize = (aabbMax - aabbMin) * 0.5;
    vec3 centerSurface = surfacePosition - (aabbMax + aabbMin) * 0.5;

    vec3 normal = vec3(0.0);
    normal += vec3(sign(centerSurface.x), 0.0, 0.0) * step(abs(abs(centerSurface.x) - halfSize.x), EPSILON);
    normal += vec3(0.0, sign(centerSurface.y), 0.0) * step(abs(abs(centerSurface.y) - halfSize.y), EPSILON);
    normal += vec3(0.0, 0.0, sign(centerSurface.z)) * step(abs(abs(centerSurface.z) - halfSize.z), EPSILON);
    return normalize(normal);
}

vec3 GetCosWeightedHemissphereDir(inout uint rndSeed, vec3 normal)
{
    float z = GetRandomFloat01(rndSeed) * 2.0 - 1.0;
    float a = GetRandomFloat01(rndSeed) * 2.0 * PI;
    float r = sqrt(1.0 - z * z);
    float x = r * cos(a);
    float y = r * sin(a);

    // Convert unit vector in sphere to a cosine weighted vector in hemissphere
    return normalize(normal + vec3(x, y, z));
}

vec2 GetPointOnCircle(inout uint rndSeed)
{
    float angle = GetRandomFloat01(rndSeed) * 2.0 * PI;
    float r = sqrt(GetRandomFloat01(rndSeed));
    return vec2(cos(angle), sin(angle)) * r;
}

uint GetPCGHash(inout uint seed)
{
    seed = seed * 747796405u + 2891336453u;
    uint word = ((seed >> ((seed >> 28u) + 4u)) ^ seed) * 277803737u;
    return (word >> 22u) ^ word;
}

float GetRandomFloat01(inout uint state)
{
    return float(GetPCGHash(state)) / 4294967296.0;
}

float GetSmallestPositive(float t1, float t2)
{
    // Assumes at least one float > 0
    return t1 < 0 ? t2 : t1;
}

float FresnelSchlick(float cosTheta, float n1, float n2)
{
    float r0 = (n1 - n2) / (n1 + n2);
    r0 *= r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosTheta, 5.0);
}

Ray GetWorldSpaceRay(mat4 inverseProj, mat4 inverseView, vec3 viewPos, vec2 normalizedDeviceCoords)
{
    vec4 rayEye = inverseProj * vec4(normalizedDeviceCoords.xy, -1.0, 0.0);
    rayEye.zw = vec2(-1.0, 0.0);
    return Ray(viewPos, normalize((inverseView * rayEye).xyz));
}