#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec4 pos0;
varying vec3 normal0;

uniform mat4 transform;
uniform mat4 posiform;

void main(){
	gl_Position = transform * vec4(position, 1.0);
	texCoord0 = texCoord;
	pos0 = posiform * vec4(position, 1.0);
	normal0 = normalize(vec3(posiform * vec4(normal, 1.0) - pos0));
	//normal0 = vec4(normal, 1.0);
}