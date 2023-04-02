#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord; // 颜色变量的属性位置值为 1
out vec3 outColor;
out vec2 texCoord;
uniform float xOffSize;
void main() {
  gl_Position = vec4(aPos.x + xOffSize, aPos.y, aPos.z, 1.0f);
  outColor = aColor;
  texCoord = aTexCoord * 2.0f - 0.5f;
};