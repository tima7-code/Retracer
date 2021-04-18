#version 130

const int obj_number = 10;

uniform vec3 obj_coord[obj_number];
uniform vec3 obj_color[obj_number];
uniform float obj_name[obj_number];
uniform float obj_size[obj_number];
uniform float obj_ref [obj_number];

uniform vec3 source_coord[obj_number];
uniform vec3 source_color[obj_number];
uniform float source_size[obj_number];
uniform int            sources_number;

uniform vec3 cam;
uniform vec2 mouse;
uniform vec2 u_resolution;

int max_dist = 99999;
vec3 camera = cam;
int ref_number = 100;

vec3 Raycast(inout vec3 rd, inout float reflectivity);
vec3 Raytrace(vec3 rd);

vec2 intersections(in int i, in vec3 rd);
vec2 sphere       (in int i, in vec3 rd);
vec2 cube         (in int i, in vec3 rd);
vec2 plane        (in int i, in vec3 rd);

vec3 object_normal(in int i, in vec3 rd, in vec2 v);

mat2 rotation(float phi);

void main()
{
	vec2 xy = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;

    vec3 ray_direction = normalize(vec3(xy, -1.0));
    ray_direction.zy *= rotation(-mouse.y);
    ray_direction.xz *= rotation(mouse.x);
    
    gl_FragColor = vec4(Raytrace(ray_direction), 1.0);
}

vec3 Raycast(inout vec3 rd, inout float reflectivity)
{
    vec3 normal;
    vec2 v = vec2(0.0);
    vec2 min_v = vec2(max_dist);
    vec3 sky_color = vec3(0.8, 1.0, 1.0);
    vec3 color = vec3(0.0);

    reflectivity = 0.0;

    for(int i = 0; i < obj_number; i++)
    {
        v = intersections(i, rd);
        if(v.x >= 0.0 && v.x < min_v.x) 
        {
            min_v = v;
            color = obj_color[i];
            reflectivity = obj_ref[i];
            normal = object_normal(i, rd, v);
        }
    }

    if (min_v.x == max_dist) return sky_color;
    camera += rd * (min_v.x - 0.001);
    if (reflectivity != 0.0) rd = reflect(rd, normal);
    return color;
}

vec3 Raytrace(in vec3 rd)
{
    float reflectivity = 0.0;
    vec3 color = Raycast(rd, reflectivity);

    if(reflectivity != 0.0)
    {
        for(int i = 0; i < ref_number - 1; i++)
        {   
            if (reflectivity != 0.0)
                color *= reflectivity * Raycast(rd, reflectivity);
            else
            {
                color *= Raycast(rd, reflectivity);
                break;
            }
        }
    }

    return color;
}

vec2 intersections(in int i, in vec3 rd)
{
    if (obj_name[i] == 1.0) return sphere(i, rd);
    if (obj_name[i] == 2.0) return cube  (i, rd);
    if (obj_name[i] == 3.0) return plane (i, rd);
}

vec3 object_normal(in int i, in vec3 rd, in vec2 v)
{
    if (obj_name[i] == 1.0) return normalize(camera + rd * v.x - obj_coord[i]);
    if (obj_name[i] == 2.0)
    {
        vec3 t1 = (-1.0/rd * (camera - obj_coord[i]) - abs(1.0/rd) * obj_size[i]);
        return normalize(-sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz));
    }
    if (obj_name[i] == 3.0) return normalize(obj_coord[i]);
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
    return vec2(-(dot(camera, obj_coord[i])+1.0)/dot(rd,obj_coord[i]));
}

mat2 rotation(float phi)
{
    float a = sin(phi);
    float b = cos(phi);
    return mat2(b, -a, a, b);
}