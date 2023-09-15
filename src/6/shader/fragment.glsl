#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 texCoord;

void main() {
  FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
  // FragColor = vec4(outColor, 1.0f);
  // FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2( -texCoord.x, texCoord.y)), wsValue);
};