# Ray Tracer

Ray-Tracer built for the Graphic Processing course @CIn - UFPE

Este é um simples ray caster implementado em C++ usando a biblioteca glm.

A branch `solid_texture_planes` implementa a adição de texturas sólidas baseadas em imagens nos planos do RayTracer, além de classes e métodos de textura necessárias para essa feature.

Imagens demonstrativas:

![demo1](https://i.imgur.com/kF2iH9q.png)
![demo2](https://i.imgur.com/tyWJ6at.png)
![demo3](https://i.imgur.com/2HVFXS7.png)


## Como compilar e executar

1. Certifique-se de ter um compilador C++ instalado no seu sistema. Recomendamos o g++ para sistemas Unix/Linux e o MinGW para Windows.

2. Clone este repositório para o seu sistema:

3. Navegue até o diretório do projeto:

    ```bash
    cd seu/caminho/aqui
    ```

4. Compile o programa usando o seguinte comando:

    ```bash
    g++ -o ray_tracer Projeto/main.cpp Projeto/Includes/ray.cpp Projeto/Includes/camera.cpp Projeto/Includes/sphere.cpp Projeto/Includes/plane.cpp Projeto/Includes/hitable_list.cpp Projeto/Includes/color.cpp Projeto/Includes/triangle.cpp Projeto/Includes/trianglemesh.cpp Projeto/Includes/transform.cpp Projeto/Includes/light.cpp Projeto/Includes/environment.cpp Projeto/Includes/material.cpp Projeto/Includes/texture.cpp

    ```

5. Execute o programa gerando um arquivo de imagem PPM:

    ```bash
    ./ray_tracer > output.ppm
    ```

### ou no Windows :

```bash
Opcão : PowerShell
.\file_name.exe > output.ppm

Opcão : cmd (recomendável)
file_name.exe > output.ppm
```

## Como visualizar arquivos .ppm

No windows, o visualizador de fotos padrão não suporta visualização de arquivos .ppm. Duas recomendações de aplicativos de terceiros para visualizar esses arquivos são:

1 - [IrfanView](https://www.irfanview.com/)

2 - [XnView](https://www.xnview.com/en/)
