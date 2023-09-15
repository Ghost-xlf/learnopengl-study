#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 outTexCoord;
uniform sampler2D texture1;

void main() {
  // FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
  // FragColor = vec4(outColor, 1.0f);
  FragColor = texture(texture1, outTexCoord);
};