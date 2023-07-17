#version 400 core
out vec4 FragColor;

uniform vec2 screensize;
uniform dvec2 coords;
uniform double zoom;
uniform float itr;
uniform float angle;

#define PI 3.14159265358979

dvec3 color(float s)
{
    double v=1.0-cos(PI*s)*cos(PI*s);
    return dvec3(v*1.0,v*2.0,v*5.0);
    //return dvec3(0.5-cos(s*75.0)/2.0,0.5-cos(s* 120.0)/2.0,0.5-cos(s*165.0)/2.0);
}

dvec3 julia(dvec2 uv)
{
    float R=2.0;
    dvec2 c=uv*5;
    c/=zoom;
    c+=coords;
    dvec2 z=dvec2(c);
    for(float i;i<itr;i++){
        z=dvec2((z.x*z.x-z.y*z.y)+(0.7885 * cos(angle)), (2.0*z.x*z.y)-(sin(angle)))+c;
        if(dot(z,z)>4.0)return color(i/itr);
    }
    
    return dvec3(0.0);
}

void main()
{
    dvec2 uv=(gl_FragCoord.xy-0.5*screensize.xy)/screensize.y;
    dvec3 col=dvec3(0.0);
    dvec3 j=julia(uv);
    col+=j;
    FragColor=vec4(col,1.0);
}