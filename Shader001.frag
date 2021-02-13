/*---------------- "exampleFragmentShader.frag" ----------------*/

// #version 150 // Specify which version of GLSL we are using.
#version 330 compatibility

precision highp float; // Video card drivers require this line to function properly

out vec4 fragColor;
varying vec4 v_NxtColor;

void main() 
{
    // fragColor = vec4(1.0, .2, 0.1, 1.0); //Set colour of each fragment to ...
	fragColor = v_NxtColor + vec4(0.f, 0.f, .2f, 0.f);

}
/*--------------------------------------------------------------*/