#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(binding = 0) uniform ubo {
	mat4 mvp;
} uboobj;

void main() {

		
    gl_Position = uboobj.mvp * vec4(inPosition, 1.0);
	fragTexCoord = inTexCoord;
}