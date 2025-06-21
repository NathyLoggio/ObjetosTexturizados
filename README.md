# Introdução à OpenGL Moderna – Mapeamento de Texturas
# Aluna: Nathaly Loggiovini  

## Descrição da Atividade

Este projeto consiste na atividade proposta para a disciplina de Processamento Gráfico, cujo objetivo é praticar o mapeamento de texturas em sprites utilizando OpenGL moderno (com shaders). Foi solicitado desenhar uma cena composta por vários retângulos texturizados (sprites) com diferentes imagens/texturas, organizados de forma a simular uma pequena cena 2D.

---

## Lógica e Organização do Projeto

### 1. Estrutura do Projeto

- **assets/**: Contém as imagens e sprites utilizados na cena (`background.png`, `Banana.png`, `Lola.png`, `Lucas.png`, etc.).
- **src/**: Código-fonte principal (`main.cpp`, `Sprite.cpp`, `Sprite.h`).
- **shaders/**: Arquivos de shader GLSL para vertex e fragment.
- **common/** e **include/**: Headers/libraries auxiliares como GLAD.

### 2. Lógica do Código

#### Carregamento de Texturas

- Utilizamos a biblioteca **stb_image.h** para carregar imagens PNG como texturas OpenGL.
- Cada imagem é carregada para um `GLuint` (ID da textura).

#### Classe Sprite

- **Reutilização de Código:** 

Criamos uma classe `Sprite` para encapsular toda a lógica de desenho de um retângulo texturizado:
  - Armazena VAO, textura, shader, posição, escala e rotação.
  - Permite criação fácil de vários sprites com diferentes texturas, tamanhos e posições.
- O método `draw()` cuida da configuração dos uniforms no shader e do envio da matriz de transformação.

#### Organização da Cena

- A matriz de projeção ortográfica foi configurada para que as coordenadas do mundo coincidam com os pixels da janela.
- O fundo é desenhado primeiro, ocupando toda a tela.
- Os demais sprites (personagens e objetos) são desenhados em posições e tamanhos variados, compondo a cena conforme o enunciado.

#### Shaders

- O vertex shader recebe a matriz de transformação do modelo e projeção, além dos uniforms para manipulação de possíveis spritesheets.
- O fragment shader aplica a textura ao sprite.

## Obrigada 