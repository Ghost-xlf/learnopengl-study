#version 330 core
out vec4 FragColor;
in vec3 outColor;
// 
uniform sampler2D texture2D;
void main() {

  FragColor = vec4(outColor, 1.0f);
};