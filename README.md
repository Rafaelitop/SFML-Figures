# SFML + ImGui Shape Viewer

Este proyecto es una aplicación en C++ que dibuja **círculos** y **rectángulos** con texto utilizando **SFML** y permite modificar sus propiedades en tiempo real con **ImGui**.

Puedes controlar:
- Posición y velocidad de las figuras.
- Escala.
- Color.
- Texto/nombre de la figura.
- Visibilidad.

---

## Requisitos

- C++17 o superior.
- [SFML 2.5+](https://www.sfml-dev.org/download.php)
- [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml)
- Un compilador compatible con C++17 (MSVC, GCC, Clang).

---

## Archivos del proyecto

- `main.cpp` → Código principal.
- `prompt.txt` → Archivo de configuración para ventana, fuentes y figuras.
- Fuentes (TTF) → necesarias para mostrar el texto de las figuras.
- `.gitignore` → Ignora archivos binarios y compilados.

---

## Configuración del archivo `prompt.txt`

El archivo controla la ventana, la fuente y las figuras. Ejemplo:
