## 绘制三角行知识总结

--- 渲染管线的介绍 - 3d 坐标 -> 2d 坐标 -> 光栅化 绘制像素
----- 阶段 顶点数组 -> 顶点着色器 -> 图元装配 -> 几何着色器（裁剪，屏幕映射，生成新顶点， 插值） - > 光栅化(深度检测--- 抗锯齿) -> 片元着色器

--- 顶点着色器获取顶点数组， 创建 VBO glGenBuffers 创建 -> glBindBuffer 绑定 -> glBufferData 数据填充 （程序完成后即时删除）
--- 顶点着色器 片元着色器 的创建 绑定 编译 校验
--- 链接顶点数据 ，顶点属性指针 glVertexAttribPointer
--- VAO 顶点数组对象 创建 绑定
--- 索引缓冲区 EBO （可以减少重复数据的复现）