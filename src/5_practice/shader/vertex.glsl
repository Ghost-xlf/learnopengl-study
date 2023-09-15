#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord; // 颜色变量的属性位置值为 1
out vec3 outColor;
out vec2 texCoord;
uniform float xOffSize;
// uniform float factor;

uniform mat4 trans;

mat4 rotate3d(float _angle) {
  return mat4 (
    cos(_angle), -sin(_angle), 0.0f, 0.0f,
    sin(_angle), cos(_angle), 0.0f,  0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
}

void main() {
  // gl_Position = vec4(rotate3d(factor)* vec4(aPos, 1.0f));
  gl_Position = vec4(trans * vec4(aPos, 1.0f));
  outColor = aColor;
  texCoord = aTexCoord * 2.0f - 0.5f;
};