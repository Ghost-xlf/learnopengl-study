#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float wsValue;

void main() {
  // FragColor = vec4(outColor, 1.0f);
  FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2( -texCoord.x, texCoord.y)), wsValue);
};