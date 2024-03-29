#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int clearColorBuffer();

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float wsValue = 0.0f;

int main()
{
  glfwInit();
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

  Shader ourShade("./src/5_practice/shader/vertex.glsl", "./src/5_practice/shader/fragment.glsl");

  glEnable(GL_PROGRAM_POINT_SIZE); // 启用后在绘制时设置点大小
  // 定义顶点数组
  float vertices[] = {
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
  };

  unsigned int indices[] = {
      // 注意索引从0开始!
      // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
      // 这样可以由下标代表顶点组合成矩形

      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };
  // 生成vbo顶点缓冲对象
  unsigned int VBO, VAO, EBO;
  // std::cout << VBO;
  glGenBuffers(1, &VBO); //& 代指引用目标地址 不构建新的内存  ！权限不能放大。   const 只读 int 可读写
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  // 绑定vao
  glBindVertexArray(VAO);

  // 绑定缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 填充数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 填充EBO数据
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // 启用顶点指针索引

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // 注意第一个参数
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0); // 解绑vb
  // 创建顶点着色器 片元着色器
  // 绘制模式限定
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框绘制
  float xOffSize = 0.0f;

  float factor;

  // 创建纹理对象texture2
  unsigned texture1, texture2;
  stbi_set_flip_vertically_on_load(true); // 纹理采样y轴取反
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // 加载图片

  int width, height, nrChannels;
  unsigned char *data = stbi_load("./static/texture/wall.jpg", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  };
  stbi_image_free(data);

  // 设置过滤方式
  float borderColor[] = {0.1f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // texture2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  };
  stbi_image_free(data);

  // 设置环绕和过滤方式

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // glUniform1i(glGetUniformLocation(ourShade.ID, "texture1"), 0);
  // ourShade.setInt("texture2", 1);

  // /**
  //  * 初始化 4分量向量
  //  */
  // glm::vec4 position = glm::vec4(1.0, 1.0, 1.0, 1.0);
  // /**
  //  * 初始一个单位矩阵
  //  */
  // glm::mat4 mat = glm::mat4(1.0f);

  // // 向右平移
  // mat = glm::translate(mat, glm::vec3(1.0f, 0.0f, 0.0f));
  // position = mat * position;

  // // 缩放
  // mat = glm::scale(mat, glm::vec3(2.0f, 3.0f, 2.0f));
  // position = mat * position;

  // // 旋转
  // mat = glm::rotate(mat, float(glm::radians(45.0)), glm::vec3(1.0f, 0.0, 0.0));
  // position = mat * position;

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, float(glm::radians(45.0)), glm::vec3(1.0f, 0.0, 0.0));
  trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
  // 矩阵传递给顶点着色器
  unsigned int transfrom = glGetUniformLocation(ourShade.ID, "trans");
  glUniformMatrix4fv(transfrom, 1, GL_FALSE, glm::value_ptr(trans));

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    clearColorBuffer();
    glBindVertexArray(VAO); // 在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。
    ourShade.use();
    glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture2);
    factor = glfwGetTime();
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f)); // 根据矩阵左乘 最后一步的矩阵操作为最先 操作 所以是先旋转 后 平移
    trans = glm::rotate(trans, float(glm::radians(factor * 5.0)), glm::vec3(0.0f, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(transfrom, 1, GL_FALSE, glm::value_ptr(trans));
    trans = glm::mat4(1.0f);

    ourShade.setInt("texture1", 0);
    ourShade.setInt("texture2", 1);
    // processInput(window);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    ourShade.setFloat("xOffSize", xOffSize);
    ourShade.setFloat("wsValue", wsValue);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // opengl核心模式要求我们使用vao 如果我们vao绑定失败 opengl会拒绝绘制任何东西
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(transfrom, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    trans = glm::mat4(1.0f);
    glBindVertexArray(0); // 解绑vao
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 资源释放
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  // std::cout << "" << std::endl;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 如果有按键输入 esc 关闭窗口
    glfwSetWindowShouldClose(window, true);

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
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  return 0;
}
