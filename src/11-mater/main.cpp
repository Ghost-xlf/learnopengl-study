#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <tool/camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
// void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_calback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double x, double y);
int clearColorBuffer();

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>
#include <geometry/SphereGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/BoxGeometry.h>

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
float wsValue = 0.0f;
using namespace std;
ImVec4 clear_color = ImVec4(0.1, 0.1, 0.1, 1.0);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);    // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机朝向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // 摄像机上方向
Camera camera(glm::vec3(0.0, 1.0, 5.0));

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float lastX = SCR_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCR_HEIGHT / 2.0f;

int main()
{
  glfwInit();
  const char *glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // opengl 模型 核心模式

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // 窗口创建
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate(); // 结束线程
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册窗口监听 第一次显示同时调用

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // 判断 GLAD 是否初始化成功 初始化glad 所有glad操作在其后面
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 鼠标键盘事件
  // 1.注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // 2.鼠标事件
  // glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_calback);
  glfwSetScrollCallback(window, scroll_callback);

  Shader ourShade("./src/11-mater/shader/vertex.glsl", "./src/11-mater/shader/fragment.glsl");
  Shader lightObjectShader("./src/11-mater/shader/light_object_vert.glsl", "./src/11-mater/shader/light_object_frag.glsl");

  SphereGeometry sphereGeometry(0.2, 10.0, 10.0);

  // PlaneGeometry planeGeometryp(1.0, 1.0);

  BoxGeometry boxGeometry(1.0);
  unsigned texture1, texture2;
  stbi_set_flip_vertically_on_load(true); // 纹理采样y轴取反
  glGenTextures(1, &texture1);            // 此处的索引标识符不是纹理单元是生成的纹理名称
  glActiveTexture(GL_TEXTURE1);           // 如果此地不绑定对应纹理单元  默认绑定至 GL_TEXTURE0
  glBindTexture(GL_TEXTURE_2D, texture1);
  int width, height, nrChannels;
  unsigned char *data = stbi_load("./static/texture/wall.jpg", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  };
  stbi_image_free(data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glGenTextures(1, &texture2);
  // glBindTexture(GL_TEXTURE_2D, texture2);
  // data = stbi_load("./static/texture/wall.png", &width, &height, &nrChannels, 0);awesomeface

  // if (data)
  // {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  // };
  // stbi_image_free(data);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // // set texture filtering parameters
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_PROGRAM_POINT_SIZE); // 启用后在绘制时设置点大小
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f)};
  // glm::mat4 model = glm::mat4(1.0f);
  // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  float factor;
  // std::cout << "1231231233123333333" << std::endl;
  cout << sizeof(cubePositions) / sizeof(*cubePositions) << endl;
  // 创建imgui上下文
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // 设置样式
  ImGui::StyleColorsDark();
  // 设置平台和渲染器
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  float fov = 45.0f; // 视口角度
  glm::vec3 view_tran = glm::vec3(0.0, 0.0, -3.0);

  glm::vec3 lightPosition = glm::vec3(1.0, 1.0, 0.0); // 光照位置
  glm::vec3 lightColor = glm::vec3(0.0, 1.0, 1.0);    // 光照颜色
  float ambientStrength = 0.2f;

  while (!glfwWindowShouldClose(window))
  {
    factor = glfwGetTime();
    deltaTime = factor - lastFrame;
    lastFrame = factor;

    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("hellow");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    // ImGui::SliderFloat("float", &fov, 0.0f, 90.0f);
    // ImGui::SliderInt("SCR_WIDTH", &SCR_WIDTH, 1, 1920);
    // ImGui::SliderInt("SCR_HEIGHT", &SCR_HEIGHT, 1, 1080);

    ImGui::SliderFloat("高光", (float *)&ambientStrength, 0.0f, 2.0f);

    ImGui::ColorEdit3("lightColor", (float *)&lightColor);
    ImGui::End();

    clearColorBuffer();
    ourShade.use();
    // glBindVertexArray(planeGeometryp.VAO);
    glBindVertexArray(boxGeometry.VAO);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 poj = glm::mat4(1.0f);
    // model = glm::rotate(model, factor * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    // 视图矩阵
    // view = glm::translate(view, view_tran);
    float radius = 10.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;

    // lookAt 1. 相机位置 2. 相机方向 3. 相机上方向
    // view = glm::lookAt(cameraPos, cameraFront, cameraUp);
    view = camera.GetViewMatrix();

    // view = glm::translate(view, view_tran);

    poj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glm::vec3 lightPos = glm::vec3(lightPosition.x * glm::sin(glfwGetTime()), lightPosition.y, lightPosition.z);
    // ourShade.setMat4("model", model);
    ourShade.setMat4("view", view);
    ourShade.setMat4("poj", poj);
    ourShade.setInt("texture1", 1);
    // ourShade.setVec3("lightColor", lightColor);
    ourShade.setVec3("viewPos", camera.Position);
    ourShade.setFloat("ambientStrength", ambientStrength);
    ourShade.setVec3("lightPos", lightPos); // 注意不能写在下方  glDrawElements 后输入的参数会是下一个着色器片段
    ourShade.setVec3("material.ambient", lightColor);
    ourShade.setVec3("material.diffuse", lightColor);
    ourShade.setVec3("material.specular", lightColor);
    ourShade.setFloat("material.shininess", 32.0f);
    for (unsigned int i = 0; i < (sizeof(cubePositions) / sizeof(*cubePositions)); i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      if (i % 3 == 0)
      {
        model = glm::rotate(model, factor * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
      }
      // float angle = 20.0f;
      // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 0.3, 0.5));
      ourShade.setMat4("model", model);
      glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }

    // 绘制灯光物体
    lightObjectShader.use();
    model = glm::mat4(1.0f);
    // cout << "lightPos" << lightPos << endl;
    model = glm::translate(model, lightPos);
    lightObjectShader.setMat4("model", model);
    lightObjectShader.setMat4("view", view);
    lightObjectShader.setMat4("projection", poj);
    lightObjectShader.setVec3("lightColor", lightColor);
    glBindVertexArray(sphereGeometry.VAO);
    glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    // glDrawElements(GL_POINTS, planeGeometryp.indices.size(), GL_UNSIGNED_INT, 0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window); // 交换颜色缓冲 并且将会作为输出显示在屏幕上。
    glfwPollEvents();
    // glDrawElements(GL_LINE_LOOP, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
  }

  // 资源释放
  boxGeometry.dispose();
  sphereGeometry.dispose();
  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window)
{
  // std::cout << "" << std::endl;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 如果有按键输入 esc 关闭窗口
    glfwSetWindowShouldClose(window, true);
  // float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
  // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  //   cameraPos += cameraSpeed * cameraFront;
  // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  //   cameraPos -= cameraSpeed * cameraFront;
  // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  //   cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  //   cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    wsValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    if (wsValue >= 1.0f)
      wsValue = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    wsValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    if (wsValue <= 0.0f)
      wsValue = 0.0f;
  }
}

// 鼠标移动监听
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_calback(GLFWwindow *window, int button, int action, int mods)
{

  if (action == GLFW_PRESS)
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
      // cout << "mouse left" << endl;
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      // cout << "mouse middle" << endl;
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      // cout << "mouse right" << endl;
      break;
    }
}

void scroll_callback(GLFWwindow *window, double x, double y)
{
  // cout << "x " << x << endl;
  // cout << "y " << y << endl;
  return;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) // 窗口改变回调
{
  glViewport(0, 0, width, height); // 设置大小显示数据和坐标 gl 前两个参数控制窗口左下角的位置   范围映射（0，1）
}

int clearColorBuffer()
{
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return 0;
}
