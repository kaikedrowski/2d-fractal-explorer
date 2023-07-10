#version 400 core
out vec4 FragColor;

uniform vec2 screensize;
uniform dvec2 coords;
uniform double zoom;
uniform float itr;

#define PI 3.14159265358979

float log10(double x){
    float y=float(x);
    return (1.0 / log(10.0)) * log(y);
}

dvec3 color(float s)
{
    double v=1.0-cos(PI*s)*cos(PI*s);
    return dvec3(v*1.0,v*2.0,v*5.0);
}

dvec3 mandelbrot(dvec2 uv)
{
    dvec2 c=5.0*uv-dvec2(0.7,0.0);
    c/=zoom;
    c+=coords;
    vec2 z=vec2(0.0);

    for(float i;i<itr;i++){
        z=vec2(z.x*z.x-z.y*z.y,2.0*z.x*z.y)+vec2(c);
        if(dot(z,z)>4.0){
            float interpolate=(i-1.0)-log(((log(dot(z,z)))/log(2.0)))/log(2.0);
            return dvec3(0.01*interpolate,0.02*interpolate,0.05*interpolate);
        }
    }
    
    return dvec3(0.0);
}

void main()
{
    dvec2 uv=(gl_FragCoord.xy-0.5*screensize.xy)/screensize.y;
    dvec3 col=dvec3(0.0);
    dvec3 m=mandelbrot(uv);
    col+=m;
    FragColor=vec4(col,1.0);
}