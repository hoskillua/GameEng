#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 position;
    bool is_opaque;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //DONE: (Req 6) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color
    if(fs_in.is_opaque)
    {
    float my_light =  max(0,dot(fs_in.normal, vec3(0,1,0)));
    frag_color = tint * texture(tex, fs_in.tex_coord) * fs_in.color * my_light ; 
    }
    else
        frag_color = tint * texture(tex, fs_in.tex_coord) * fs_in.color ; 

    // frag_color = vec4(0.0, 0.0, 0.0, 1.0);
}