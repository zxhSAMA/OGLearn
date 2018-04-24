#version 450
out vec4 fragColor;
//uniform float gColor;
in vec4 color;
void main() {
	//fragColor = vec4(1.0-gColor,gColor, 0.3, 1.0);
	fragColor = color;
}