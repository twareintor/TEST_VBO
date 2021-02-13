/*----------------- "exampleVertexShader.vert" -----------------*/

// #version 150 // Specify which version of GLSL we are using.
#version 330 compatibility


// in_Position was bound to attribute index 0 ("shaderAttribute")
in  vec3 in_Position;
in  vec3 in_Scale;
varying vec4 v_NxtColor;

void main() 
{
    gl_Position = vec4(in_Position.x*.1,1 in_Position.y*.7, in_Position.z*.1, 1.0);
    v_NxtColor = vec4(1.f-normalize(gl_Position).x, 1.f-normalize(gl_Position).y, normalize(gl_Position).z, .0);

}
/*--------------------------------------------------------------*/
