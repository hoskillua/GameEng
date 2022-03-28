#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the center and the radius
uniform vec2 center = vec2(256,256);  //center of the circle as sent from the json file
uniform float radius = 128;          //radius of the circle as sent from the json file

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the circle

    // calculate the euclidian distance betwween the point and the center of the circle
    float eucliden_dist = sqrt(pow((gl_FragCoord.x-center[0]),2) + pow((gl_FragCoord.y-center[1]),2) );
    if(eucliden_dist <= radius){  // less than radius means the point is inside the circle
        frag_color = inside_color;
    } else {  // greater than radius means the point is outside the circle
        frag_color = outside_color;
    }
}