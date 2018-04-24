#version 450
layout (location = 0) in vec3 Position;
//uniform float gScale;
uniform mat4 gWorld;
out vec4 color;
void main() {
	//gl_Position = vec4(0.7*Position.x, 0.7 * Position.y, 0.7*Position.z, 1.0);
	//gl_Position = vec4(gScale*Position.x, gScale*Position.y, Position.z, 1.0);
	gl_Position = gWorld * vec4(Position, 1.0);
	color = vec4(clamp(Position,0.0,1.0),1.0);
}