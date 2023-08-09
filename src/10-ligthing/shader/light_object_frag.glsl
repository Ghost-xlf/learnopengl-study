#version 330 core

uniform vec3 lightColor;

out vec4 FragColor;
in vec2 outTexCoord;

void main() {
  FragColor = vec4(lightColor,1);
}