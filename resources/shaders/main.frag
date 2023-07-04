#version 330 core
out vec4 FragColor;

uniform vec2 screensize;
uniform vec2 coords;
uniform float zoom;
uniform float itr;

#define PI 3.141592653589

vec3 color(float s)
{
    float v=1.0-cos(PI*s)*cos(PI*s);
    return vec3(v*1.0,v*2.0,v*5.0);
}

vec3 mandelbrot(vec2 uv)
{
    vec2 c=5.0*uv-vec2(0.7,0.0);
    c/=zoom;
    c+=coords;
    vec2 z=vec2(0.0);
    float iter=0.0;
    for(float i;i<itr;i++){
        z=vec2(z.x*z.x-z.y*z.y,2.0*z.x*z.y)+c;
        if(dot(z,z)>4.0)return color(iter/itr);
        iter++;
    }
    return vec3(0.0);
}

void main()
{
    vec2 uv=(gl_FragCoord.xy-0.5*screensize.xy)/screensize.y;
    vec3 col=vec3(0.0);
    vec3 m=mandelbrot(uv);
    col+=m;
    FragColor=vec4(col,1.0);
}