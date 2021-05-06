#version 130

vec3 obj_coord[2];
uniform vec3 obj_color[2];
uniform float obj_name[2];
uniform float obj_size[2];
uniform vec3  obj_prop[2];
uniform vec2  phi;

uniform sampler2D u_skybox;

vec3 camera = vec3(0, 0, 600);
uniform vec2 u_resolution;

int max_dist = 5000;
int ref_number = 2;

vec3 Raytrace(vec3 rd);
vec3 Raycast(inout vec3 rd, inout float reflectivity, inout float refraction);
vec3 getSky(vec3 rd);

vec2 intersections(in int i, in vec3 rd);
vec2 sphere       (in int i, in vec3 rd);
vec2 cube         (in int i, in vec3 rd);
vec2 plane        (in int i, in vec3 rd);

vec3 object_normal(in int i, in vec3 rd, in vec2 v);

mat2 rotation(float phi);

void main()
{
    obj_coord[0] = vec3(0, 700, 0);
    obj_coord[1] = vec3(0, 0, 0);
    
	vec2 xy = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;

    vec3 ray_direction = normalize(vec3(xy, -1.0));

    camera.zy *= rotation(-phi.y);
    camera.xz *= rotation(phi.x);

    ray_direction.zy *= rotation(-phi.y);
    ray_direction.xz *= rotation(phi.x);
    
    gl_FragColor = vec4(Raytrace(ray_direction), 1.0);
}

vec3 Raycast(inout vec3 rd, inout float reflectivity, inout float refraction)
{
    vec3 normal;
    vec2 v = vec2(0.0);
    vec2 min_v = vec2(max_dist);
    vec3 sky_color = vec3(0.8, 1.0, 1.0);
    vec3 color = vec3(0.0);

    refraction   = 0.0;
    reflectivity = 0.0;

    for(int i = 0; i < 2; i++)
    {
        v = intersections(i, rd);
        if(v.x >= 0.0 && v.x < min_v.x) 
        {
            min_v = v;
            color = obj_color[i];
            reflectivity = obj_prop[i].x;
            refraction   = obj_prop[i].y;
            normal = object_normal(i, rd, v);
        }
    }

    if (min_v.x == max_dist) return getSky(rd);

    if (reflectivity != 0.0)
    {
        camera += rd * (min_v.x - 0.001);
        rd = reflect(rd, normal);
    }
    if (refraction   != 0.0) 
    {
        camera += rd * (min_v.y + 0.001);
        rd = refract(rd, normal, 1.0 / refraction);
    }
    return color;
}

vec3 Raytrace(in vec3 rd)
{
    float reflectivity = 0.0;
    float refraction   = 0.0;

    vec3 color = Raycast(rd, reflectivity, refraction);

    if(reflectivity != 0.0 || refraction != 0)
    {
        for(int i = 0; i < ref_number - 1; i++)
        {   
            if (reflectivity != 0.0)
                color *= reflectivity * Raycast(rd, reflectivity, refraction);
            else if (refraction != 0.0)
            {
                color *= Raycast(rd, reflectivity, refraction);
            }
            else
            {
                color *= Raycast(rd, reflectivity, refraction);
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

vec3 getSky(vec3 rd)
{
    vec2 uv = vec2(atan(rd.x, rd.z), 2 * asin(rd.y));
    uv /= 3.14159265;
    uv = uv * 0.5 + 0.5;
    vec3 color = texture(u_skybox, uv).rgb;
    return color;
}