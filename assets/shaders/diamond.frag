#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the center and the side-length
uniform float side_length  = 128;
uniform vec2 center = vec2(256,256);
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the diamond

    //calculating the manhatten distance
    float dist =abs(gl_FragCoord.x-center[0])+abs(gl_FragCoord.y-center[1]);
    if(dist <= side_length/2){  // this means point is inside the diamond then inside color
        frag_color = inside_color;
    } else { // this means point is outside the diamond then outside color
        frag_color = outside_color;
    }
}