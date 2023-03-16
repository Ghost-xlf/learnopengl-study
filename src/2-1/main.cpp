#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int clearColorBuffer();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                 "}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                    "}\n\0";
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

  glEnable(GL_PROGRAM_POINT_SIZE); // 启用后在绘制时设置点大小
  // 定义顶点数组
  float vertices[] = {
      -0.5f,
      0.5f,
      0.0f,
      -0.75f,
      -0.5,
      0.0f,
      -0.25f,
      -0.5f,
      0.0f,
      0.5f,
      0.5f,
      0.0f,
      0.25f,
      -0.5f,
      0.0f,
      0.75f,
      -0.5f,
      0.0f,
  };

  unsigned int indices[] = {
      // 注意索引从0开始!
      // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
      // 这样可以由下标代表顶点组合成矩形

      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };
  // 生成vbo顶点缓冲对象
  unsigned int VBOS[2], VAOS[2], EBO;
  // std::cout << VBO;
  glGenBuffers(2, VBOS); //& 代指引用目标地址 不构建新的内存  ！权限不能放大。   const 只读 int 可读写
  glGenVertexArrays(2, VAOS);
  glGenBuffers(1, &EBO);
  // 绑定vao
  glBindVertexArray(VAOS[0]);

  // 绑定缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);

  // 填充数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 填充EBO数据
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // 启用顶点指针索引

  // 绑定vao
  glBindVertexArray(VAOS[1]);

  // 绑定缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

  // 填充数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 填充EBO数据
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9 * sizeof(float)));
  glEnableVertexAttribArray(0); // 启用顶点指针索引

  glBindVertexArray(0); // 解绑vb
  // 创建顶点着色器 片元着色器
  unsigned int vertexShader, fragmentShader1, fragmentShader2;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  // 绑定着色器
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);
  // 编译检测
  int success;
  char infoLog[512]; // 字符串数组
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // 创建程序对象
  unsigned int shaderProgram1, shaderProgram2;
  shaderProgram1 = glCreateProgram();
  shaderProgram2 = glCreateProgram();
  // 绑定着色器
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);

  // glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);

  // 链接着色器

  glLinkProgram(shaderProgram1);
  glLinkProgram(shaderProgram2);

  glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM\n"
              << infoLog << std::endl;
  }

  // 使用着色器程序
  glUseProgram(shaderProgram1);
  glUseProgram(shaderProgram2);
  // 删除
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);
  // 绘制模式限定
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框绘制

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    clearColorBuffer();
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAOS[0]);       // 在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。
    glDrawArrays(GL_TRIANGLES, 0, 3); // GL_TRIANGLES 三角 GL_LINE_LOOP 线 GL_POINT 点

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOS[1]);       // 在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。
    glDrawArrays(GL_TRIANGLES, 0, 3); // GL_TRIANGLES 三角 GL_LINE_LOOP 线 GL_POINT 点
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // opengl核心模式要求我们使用vao 如果我们vao绑定失败 opengl会拒绝绘制任何东西
    // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // 解绑vao
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 资源释放
  glDeleteVertexArrays(2, VAOS);
  glDeleteBuffers(2, VBOS);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram1);
  glDeleteProgram(shaderProgram2);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  // std::cout << "" << std::endl;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 如果有按键输入 esc 关闭窗口
    glfwSetWindowShouldClose(window, true);
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