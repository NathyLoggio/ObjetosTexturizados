#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"

// Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader();
int setupSprite();
int loadTexture(const std::string& filePath);

const GLuint WIDTH = 800, HEIGHT = 800;

const GLchar* vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texc;
out vec2 tex_coord;
uniform mat4 model;
uniform mat4 projection;
uniform vec2 offset;
uniform vec2 scale;
void main()
{
    tex_coord = offset + vec2(texc.x, 1.0 - texc.y) * scale;
    gl_Position = projection * model * vec4(position, 1.0);
}
)";
const GLchar* fragmentShaderSource = R"(
#version 400
in vec2 tex_coord;
out vec4 color;
uniform sampler2D tex_buff;
void main()
{
    color = texture(tex_buff, tex_coord);
}
)";

int main()
{
    // Inicialização GLFW
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Processamento Grafico Atividade", NULL, NULL);
    if (!window)
    {
        std::cerr << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup shader, VAOs e texturas
    GLuint shader = setupShader();
    GLuint vao = setupSprite();

    // Carregue suas texturas (ajuste os caminhos para suas imagens)
    GLuint texFundo = loadTexture("../assets/background.png");
    GLuint texBanana = loadTexture("../assets/sprites/Banana.png");
    GLuint texLola = loadTexture("../assets/sprites/Lola.png");
    GLuint texLucas = loadTexture("../assets/sprites/Lucas.png");
    GLuint texPinkMonster = loadTexture("../assets/sprites/Pink_Monster.png");
    GLuint texWua = loadTexture("../assets/sprites/Wua.png");

   // Personagens e objetos espalhados pela tela
    Sprite fundo(vao, texFundo, shader, glm::vec3(WIDTH/2, HEIGHT/2, 0), glm::vec3(WIDTH, HEIGHT, 1), 0.0f, 1, 1, 1.0f);
    
    Sprite banana(vao, texBanana, shader, glm::vec3(100, 700, 0), glm::vec3(80, 80, 1), 0.0f, 1, 1, 1.0f);
    Sprite lola(vao, texLola, shader, glm::vec3(300, 500, 0), glm::vec3(100, 100, 1), 0.0f, 1, 1, 1.0f);
    Sprite lucas(vao, texLucas, shader, glm::vec3(500, 300, 0), glm::vec3(120, 120, 1), 0.0f, 1, 1, 1.0f);
    Sprite pinkMonster(vao, texPinkMonster, shader, glm::vec3(700, 100, 0), glm::vec3(90, 90, 1), 0.0f, 1, 1, 1.0f);
    Sprite wua(vao, texWua, shader, glm::vec3(650, 600, 0), glm::vec3(110, 110, 1), 0.0f, 1, 1, 1.0f);

    glm::mat4 projection = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha os sprites na ordem desejada
        fundo.draw(projection);
        banana.draw(projection);
        lola.draw(projection);
        lucas.draw(projection);
        pinkMonster.draw(projection);
        wua.draw(projection);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupShader()
{
    // ... (mantém igual ao seu código)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex_buff"), 0);

    return shaderProgram;
}

int setupSprite()
{
    GLfloat vertices[] = {
        // x    y    z    s    t
        -0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5, -0.5, 0.0, 0.0, 0.0,
         0.5,  0.5, 0.0, 1.0, 1.0,
         0.5, -0.5, 0.0, 1.0, 0.0
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int loadTexture(const std::string& filePath)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}