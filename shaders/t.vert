#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( push_constant ) uniform mvp4 {
    mat4 mvp;
} p4;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(binding = 0) uniform color {
	vec4 uColor;
} bufferColor;

void main() {
	if (inColor == vec3(1.0, 0.0, 0.0))
		fragColor = vec3(bufferColor.uColor.x, bufferColor.uColor.y, bufferColor.uColor.z);
	else
		fragColor = inColor;
		
    gl_Position = p4.mvp * vec4(inPosition, 1.0);
	fragTexCoord = inTexCoord;
}