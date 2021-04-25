#version 130
//Ñustomization----------------------------------------------------------------------------------
const int ref_number = 100; 
const int rays_number = 4;  
vec4 sky_color = vec4(0.0, 0.0, 0.0, -1.0); 
//-----------------------------------------------------------------------------------------------

//Information about objects----------------------------------------------------------------------
const int     obj_number = 10;
uniform vec3  obj_coord[obj_number];
uniform vec3  obj_color[obj_number];
uniform float obj_name[obj_number];
uniform float obj_size[obj_number];
uniform vec3  obj_prop[obj_number];
//-----------------------------------------------------------------------------------------------

//Information about light sources----------------------------------------------------------------
const int     sources_number = 10;
uniform vec3  source_coord[obj_number];
uniform vec3  source_color[obj_number];
uniform float source_size[obj_number];
//-----------------------------------------------------------------------------------------------

//Camera, mouse, screen dimensions, and different constants--------------------------------------
uniform vec3 cam;
uniform vec2 mouse;
uniform vec2 u_resolution;
uniform sampler2D u_sample;
uniform float u_sample_part;
float max_dist = 99999.0;
vec3 camera = cam;
//-----------------------------------------------------------------------------------------------

//Random-----------------------------------------------------------------------------------------
uvec4 R_STATE;
uniform vec2 seed1;
uniform vec2 seed2;
vec2 hash(vec2 p);
float random();
vec3 random_vector();
uint TausStep(uint z, int S1, int S2, int S3, uint M);
uint LCGStep (uint z, uint A, uint C);
//-----------------------------------------------------------------------------------------------

//Function prototypes----------------------------------------------------------------------------
vec3 Raytrace(vec3 rd);
vec4 Raycast(inout vec3 rd, inout float reflectivity, inout float refraction);

//intersection functions
vec2 intersections(in int i, in vec3 rd);
vec2 sphere       (in int i, in vec3 rd);
vec2 cube         (in int i, in vec3 rd);
vec2 plane        (in int i, in vec3 rd);
vec2 source       (in int i, in vec3 rd);

//normal
vec3 object_normal(in int i, in vec3 rd, in vec2 v);

//rotation
mat2 rotation(float phi);

//tonemapping
vec3 mul(const mat3 m, const vec3 v);
vec3 rtt_and_odt_fit(vec3 v);
vec3 aces_fitted(vec3 v);
//-----------------------------------------------------------------------------------------------

void main()
{
	vec2 xy = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;

    vec2 xyRes = hash(xy + 1.0) * u_resolution + u_resolution;
	R_STATE.x = uint(seed1.x + xyRes.x);
	R_STATE.y = uint(seed1.y + xyRes.x);
	R_STATE.z = uint(seed2.x + xyRes.y);
	R_STATE.w = uint(seed2.y + xyRes.y);
    
    vec3 ray_direction = normalize(vec3(xy, -1.0));
    ray_direction.zy *= rotation(-mouse.y);
    ray_direction.xz *= rotation(mouse.x);
    
    vec3 result_color = vec3(0.0);
    for(int i = 0; i < rays_number; i++)
    {
        result_color += Raytrace(ray_direction);
    }

    result_color /= rays_number;

    result_color = aces_fitted(result_color);

	vec3 sampleCol = texture(u_sample, gl_TexCoord[0].xy).rgb;
	result_color = mix(sampleCol, result_color, u_sample_part);

    gl_FragColor = vec4(result_color, 1.0);
}

vec4 Raycast(inout vec3 rd, inout float reflectivity, inout float refraction)
{
    vec3 normal;
    vec3 reflected;
    vec3 diffuse;
    vec3 rand_vec;
    vec2 v = vec2(0.0);
    vec2 min_v = vec2(max_dist);
    vec4 color;

    refraction   = 0.0;
    reflectivity = 0.0;

    for(int i = 0; i < obj_number; i++)
    {
        v = intersections(i, rd);
        if(v.x >= 0.0 && v.x < min_v.x) 
        {
            min_v = v;
            color = vec4(obj_color[i], 0.0);
            reflectivity = obj_prop[i].x;
            refraction   = obj_prop[i].y;
            normal = object_normal(i, rd, v);
        }
    }

    for(int i = 0; i < sources_number; i++)
    {
        v = source(i, rd);
        if(v.x >= 0.0 && v.x < min_v.x) 
        {
            min_v = v;
            color = vec4(source_color[i], -1.0);
        }
    }

    if (min_v.x == max_dist)  return sky_color;
    if (color.a == -1.0)      return color;

    rand_vec = random_vector();
    reflected = reflect(rd, normal);
    diffuse = normalize(rand_vec * dot(rand_vec, normal));

    if (refraction != 0.0) 
    {
        camera += rd * (min_v.y + 0.001);
        rd = refract(rd, normal, 1.0 / refraction);
    }
    else
    {
        camera += rd * (min_v.x - 0.001);
        rd = mix(diffuse, reflected, reflectivity);
    }

    return color;
}

vec3 Raytrace(in vec3 rd)
{
    vec4 ref_color;
    vec3 color = vec3(1.0);
    float reflectivity = 0.0;
    float refraction   = 0.0;

    for(int i = 0; i < ref_number; i++)
    {   
        ref_color = Raycast(rd, reflectivity, refraction);
        color *= ref_color.rgb;
        if (ref_color.a == -1.0) return color;
    }

    return vec3(0.0);
}

vec2 intersections(in int i, in vec3 rd)
{
    if (obj_name[i] == 1.0) return sphere(i, rd);
    if (obj_name[i] == 2.0) return cube  (i, rd);
    if (obj_name[i] == 3.0) return plane (i, rd);
    return vec2(-1.0);
}

vec3 object_normal(in int i, in vec3 rd, in vec2 v)
{
    if (obj_name[i] == 1.0) return normalize(camera + rd * v.x - obj_coord[i]);
    if (obj_name[i] == 2.0)
    {
        vec3 t1 = (-1.0/rd * (camera - obj_coord[i]) - abs(1.0/rd) * obj_size[i]);
        return normalize(-sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz));
    }
    if (obj_name[i] == 3.0) 
    {
        float l_coord = length(obj_coord[i]);
        float projection = dot(camera, obj_coord[i]) / l_coord;
        if (projection <= l_coord) return normalize(-obj_coord[i]);
        else                       return normalize( obj_coord[i]);
    }
}

vec2 source(in int i, in vec3 rd)
{
    if (source_color[i] == vec3(0.0)) return vec2(-1.0);
    vec3 oc = camera - source_coord[i];
    float b = dot(oc, rd);
    float c = dot(oc, oc) - source_size[i]*source_size[i];
    float h = b*b - c;
    if(h < 0.0) return vec2(-1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

vec2 cube(in int i, in vec3 rd) 
{
    vec3 m = 1.0/rd;
    vec3 n = m * (camera - obj_coord[i]); 
    vec3 k = abs(m) * obj_size[i];
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
    if( tN > tF || tF < 0.0) return vec2(-1.0); 
    return vec2( tN, tF );
}

vec2 sphere(in int i, in vec3 rd)
{
    vec3 oc = camera - obj_coord[i];
    float b = dot(oc, rd);
    float c = dot(oc, oc) - obj_size[i]*obj_size[i];
    float h = b*b - c;
    if(h < 0.0) return vec2(-1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

vec2 plane(in int i, in vec3 rd)
{
    vec3 n;
    float l_coord = length(obj_coord[i]);
    float projection = dot(camera, obj_coord[i]) / l_coord;
    if (projection > l_coord) n = normalize(-obj_coord[i]);
    else                      n = normalize( obj_coord[i]);
    return vec2(-(dot(camera - obj_coord[i], n) + 1.0) / dot(rd, n));
}

mat2 rotation(float phi)
{
    float a = sin(phi);
    float b = cos(phi);
    return mat2(b, -a, a, b);
}

uint TausStep(uint z, int S1, int S2, int S3, uint M)
{
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);	
}

uint LCGStep(uint z, uint A, uint C)
{
	return (A * z + C);	
}

vec2 hash(vec2 p)
{
	p += seed1.x;
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
	p3 += dot(p3, p3.yzx+33.33);
	return fract((p3.xx+p3.yz)*p3.zy);
}

float random()
{
	R_STATE.x = TausStep(R_STATE.x, 13, 19, 12, uint(4294967294));
	R_STATE.y = TausStep(R_STATE.y, 2, 25, 4, uint(4294967288));
	R_STATE.z = TausStep(R_STATE.z, 3, 11, 17, uint(4294967280));
	R_STATE.w = LCGStep(R_STATE.w, uint(1664525), uint(1013904223));
	return 2.3283064365387e-10 * float((R_STATE.x ^ R_STATE.y ^ R_STATE.z ^ R_STATE.w));
}

vec3 random_vector() 
{
	vec3 rand = vec3(random(), random(), random());
	float theta = rand.x * 2.0 * 3.14159265;
	float v = rand.y;
	float phi = acos(2.0 * v - 1.0);
	float r = pow(rand.z, 1.0 / 3.0);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);
	return vec3(x, y, z);
}

const mat3 aces_input_matrix =
{
    vec3(0.59719f, 0.35458f, 0.04823f),
    vec3(0.07600f, 0.90834f, 0.01566f),
    vec3(0.02840f, 0.13383f, 0.83777f)
};

const mat3 aces_output_matrix =
{
    vec3( 1.60475f, -0.53108f, -0.07367f),
    vec3(-0.10208f,  1.10813f, -0.00605f),
    vec3(-0.00327f, -0.07276f,  1.07602f)
};

vec3 mul(const mat3 m, const vec3 v)
{
    float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    float y = m[1][0] * v[1] + m[1][1] * v[1] + m[1][2] * v[2];
    float z = m[2][0] * v[1] + m[2][1] * v[1] + m[2][2] * v[2];
    return vec3(x, y, z);
}

vec3 rtt_and_odt_fit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 aces_fitted(vec3 v)
{
    float k = 2.5;
    v = k*mul(aces_input_matrix, v);
    v = k*rtt_and_odt_fit(v);
    return mul(aces_output_matrix, v);
}
