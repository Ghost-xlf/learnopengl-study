#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 aNormal;
in vec3 FragPos;
uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength; // 光强

float specularStrength = 0.5; // 反射强度

void main() {


  vec3 ambient = ambientStrength * lightColor; // 环境光

  vec3 norm = normalize(aNormal);
  vec3 lightDir = normalize(lightPos - FragPos); // 得出 物体 -> 光源 方向向量 使其与法线同一起点 点乘得出夹角

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 viewDir = normalize(viewPos - FragPos);  // 视线位置 - 物体位置 = 物体朝向 相机方向向量 归一化
  vec3 reflectDir = reflect(-lightDir, norm); // reflect 第一个参数需要的是从光源指向片段位置的向量 但lightDir向量是从物体指向光源 所以取反

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

  vec3 specular = specularStrength * spec * lightColor;

  // FragColor = vec4(lightPos,1.0f);
  // FragColor = vec4(outColor, 1.0f);
  FragColor = vec4(texture(texture1, outTexCoord).xyz * (ambient + diffuse+specular ), 1.0);
  // FragColor = vec4((ambient + diffuse), 1.0);
};