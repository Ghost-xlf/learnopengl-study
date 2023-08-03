#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

  Shader ourShade("./src/9-Camera/shader/vertex.glsl", "./src/9-Camera/shader/fragment.glsl");

  // SphereGeometry sphereGeometry(0.5, 20.0, 20.0);

  PlaneGeometry planeGeometryp(1.0, 1.0);

  BoxGeometry boxGeometry(1.0);
  unsigned texture1;
  stbi_set_flip_vertically_on_load(true); // 纹理采样y轴取反
  glGenTextures(1, &texture1);
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

  glEnable(GL_PROGRAM_POINT_SIZE); // 启用后在绘制时设置点大小
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)};
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

  while (!glfwWindowShouldClose(window))
  {
    factor = glfwGetTime();
    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("hellow");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderFloat("float", &fov, 0.0f, 90.0f);
    ImGui::SliderInt("SCR_WIDTH", &SCR_WIDTH, 1, 1920);
    ImGui::SliderInt("SCR_HEIGHT", &SCR_HEIGHT, 1, 1080);

    ImGui::SliderFloat3("view", (float *)&view_tran, -10.0f, 10.0f);

    ImGui::ColorEdit3("clear color", (float *)&clear_color);
    ImGui::End();

    clearColorBuffer();
    ourShade.use();
    glBindVertexArray(planeGeometryp.VAO);
    glBindVertexArray(boxGeometry.VAO);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 poj = glm::mat4(1.0f);
    model = glm::rotate(model, factor * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    // 视图矩阵
    // view = glm::translate(view, view_tran);
    float radius = 10.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;

    // lookAt 1. 相机位置 2. 相机方向 3. 相机上方向
    view = glm::lookAt(cameraPos, cameraFront, cameraUp);
    // view = glm::translate(view, view_tran);

    poj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    ourShade.setMat4("model", model);
    ourShade.setMat4("view", view);
    ourShade.setMat4("poj", poj);
    ourShade.setInt("texture1", 0);

    // glDrawElements(GL_POINTS, planeGeometryp.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINE_LOOP, planeGeometryp.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

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
    // glDrawElements(GL_POINTS, planeGeometryp.indices.size(), GL_UNSIGNED_INT, 0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window); // 交换颜色缓冲 并且将会作为输出显示在屏幕上。
    glfwPollEvents();
    // glDrawElements(GL_LINE_LOOP, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
  }

  // 资源释放
  planeGeometryp.dispose();
  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window)
{
  // std::cout << "" << std::endl;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 如果有按键输入 esc 关闭窗口
    glfwSetWindowShouldClose(window, true);
  float cameraSpeed = 0.05f; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
