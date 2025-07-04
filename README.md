# 8-bol-pul

Ian Kersz Amaral - 00338368
Tomás Mitsuo Dias Ueda - 00344595

## Contribuições de Cada Integrante

Parágrafo listando as contribuição de cada membro da dupla para o trabalho;

### Ian Kersz Amaral

!NÃO ESCRITO!

### Tomás Mitsuo Dias Ueda

!NÃO ESCRITO!

## Uso de IAs

!NÃO ESCRITO!

Parágrafo curto indicando se a dupla fez uso do ChatGPT (ou alguma outra ferramenta similar, como Claude, Gemini, LLaMa, Github Copilot, OpenAI Codex, etc.) para desenvolvimento do trabalho, descrevendo como a ferramenta foi utilizada e para quais partes do trabalho. O parágrafo deve também incluir uma análise crítica descrevendo quão útil a dupla achou a ferramenta, onde ela auxiliou e onde ela não auxiliou adequadamente;

## Descrição do Processo de Desenvolvimento

!NÃO ESCRITO!

Descrição do processo de desenvolvimento e do uso em sua aplicação dos conceitos de Computação Gráfica estudados e listados nos requisitos acima;

## Uso dos conceitos da disciplina

!NÃO ESCRITO COMPLETAMENTE!

No mínimo duas imagens mostrando o funcionamento da aplicação;

### Câmera

Foram implementados 2 tipos de câmera diferentes:

- Câmera Look-At apontado para a bola branca
    ![Imagem da camera focada na bola branca](imgs/look_at.png)
- Câmera Livre
    ![Imagem da camera livre](imgs/freecam.png)

### Transformações Geométricas

Bolas:
- Translação: Movimentação das bolas no espaço do jogo
- Rotação: Rotação das bolas para dar um realismo maior enquanto elas se movimentam

Taco:
- Translação: Realizado a translação do taco para permanecer em conjunto com a bola branca e tambem para modificar a posição dele em relação a bola, como subir ou abaixar, além de bater na bola de canto
- Rotação: A rotação sobre dois eixos foi utilizada para colocar o taco em um angulo não horizontal a mesa e tambem para rotaciona-lo em volta da bola

O usuario controla diretamente as transformações no Taco para efetuar as mecanicas do jogo.

Mesa:
- Translação: A mesa foi trasladada para se adequar melhor o modelo para o sistema de coordenadas global.


### Iluminação

!NÃO ESCRITO!

### Colisões

Para as colisões foram utilizados 4 tipos de testes diferentes:

- Esfera-Parede: Foi utilizado para fazer a reflexão da bola quando ela bate nas bordas da mesa.

- Esfera-Esfera: Utilizada para criar as colisões entre as bolas, formando a maior parte da jogabilidade.

- Esfera-Raio: Utilizado para testar se o taco colidio com a bola branca durante aquele frame. Inicialmente foi utilizado um teste de Esfera-Ponto, mas isso causava problemas de Tunelamente em taxas de quadros mais baixas, portanto, foi modificado para um segmento de raio com comprimento de deslocamento de um frame.

- Esfera-Circulo: Testa se as bolas entraram na area pertencentes aos buracos nos cantos da mesa.

### Curvas de Bezier

As curvas de bezier cubicas foram implementadas e utilizadas para criar uma animação suave das bolas entrando nos buracos das mesas. Essa curva faz um caminho curvo do ponto de colisão da bola até a parte inferior do buraco.

### Texturas

!NÃO ESCRITO!

## Manual de Uso

### Objetivo

Fazer com que as bolas numeradas caiam nos buracos em ordem crescente dos seus numeros.

### Movimentação e Camera

#### Teclas em Qualquer modo

##### R: Reseta o jogo para o início

##### I: Abre o menu de informações

##### Botão direito do mouse: Puxa e solta o taco

##### Botão esquerdo do mouse: Movimenta os angulos da câmera

##### Botão do meio do mouse: Centraliza o taco na bola

##### Espaço: Troca entre a camera de LookAt e Livre

#### Teclas em modo Camera LookAt

##### Scroll do mouse: Aumenta ou diminui o zoom da câmera

##### W: Move o Taco para cima em relação a bola

##### A: Move o Taco para a esquerda em relação a bola

##### S: Move o Taco para baixo em relação a bola

##### D: Move o Taco para a direita em relação a bola

#### Teclas em modo Camera Livre

##### W: Move a câmera para frente

##### A: Move a câmera para a esquerda

##### S: Move a câmera para trás

##### D: Move a câmera para a direita

##### Q: Rotaciona o taco no sentido horário

#### E: Rotaciona o taco no sentido anti-horário

## Compilação e Execução

Compilação pode ser feita através de CMake, o qual foi configurado com os arquivos corretos. Ele gera arquivos executaveis na pasta build, que podem ser executados diretamente.

Já foi disponibilizado um executável para Windows e Linux, que pode ser executado diretamente sem a necessidade de compilação.
