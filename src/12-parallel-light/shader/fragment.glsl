#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 aNormal;
in vec3 FragPos;
uniform sampler2D texture0;
uniform sampler2D texture1;
// uniform vec3 lightColor;
uniform vec3 lightPos; // 当前设置为光的方向
uniform vec3 viewPos;
uniform float ambientStrength; // 光强

float specularStrength = 0.9; // 反射强度

struct Material {
  vec3 ambient; // 环境光  颜色
  vec3 diffuse; // 漫反射
  vec3 specular; // 高光
  float shininess; // 高光指数
}; 
/**
F = 1.0 / Kc+Kl∗d+Kq∗d2

**/
struct Light  {
  float constant; // 衰减常数项 Kc
  float linear; // 衰减一次项 K1
  float quadratic; // 衰减二次项 K2
}; 



uniform Material material;

void main() {


  vec3 ambient = ambientStrength * material.ambient; // 环境光

  vec3 norm = normalize(aNormal);
  // vec3 lightDir = normalize(lightPos - FragPos); // 得出 物体 -> 光源 方向向量 使其与法线同一起点 点乘得出夹角
  vec3 lightDir = normalize( -lightPos ); // 取反 唯当前物体方向指向光源。
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * material.diffuse;

  vec3 viewDir = normalize(viewPos - FragPos);  // 视线位置 - 物体位置 = 物体朝向 相机方向向量 归一化
  vec3 reflectDir = reflect(-lightDir, norm); // reflect 第一个参数需要的是从光源指向片段位置的向量 但lightDir向量是从物体指向光源 所以取反

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // vec3 specular = specularStrength * spec * material.specular; // 镜面光
  vec3 specular = texture(texture1, outTexCoord).xyz * specularStrength * spec * material.specular; // 镜面光
  // FragColor = vec4(lightPos,1.0f);
  // FragColor = texture(texture1, outTexCoord);
  FragColor = vec4(texture(texture0, outTexCoord).xyz * (ambient + diffuse + specular), 1.0);
  // FragColor = vec4((ambient + diffuse), 1.0);

};