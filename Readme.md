# Setup

## Installing and getting started on WSL
- Prerequisits are WSL2 and Windows 11 for the integrated UI Forwarding

1. Installing the Prerequesites
```
apt install build-essential cmake libglfw3-dev libglm-dev mesa-utils mesa-common-dev
```
or alternatively for working with GLUT
```
apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
2. Export Display-Forwarding and test UI Forwarding
```
export DISPLAY=:0
xeyes +shape -biblicallyAccurate +present -geometry 800x800
```
3. Test OpenGL Display
```
glxgears
```

## Setting up the Project
- Getting the GLAD Setupfiles via\n
https://glad.dav1d.de/

    ```
    Options are:
    - C/C++
    - OpenGL
    - gl: v3.3
    - profile: Core
    - Options: generate a Loader
    ```
Click generate, download the ZIP and put it in here (include in include and glad.c next to main)

## Notes
- For NVim to properly work be sure to generate a compile_commands.json. Use ```compiledb make``` for this.
- WSL2 seems to support 16 Vertex Attributes
- based on [this](https://learnopengl.com/) great tutorial
- Classbased approach really helps abstracting the complexity, but really has to be thought through to prevent full rewrites in just the next chapter when a new option is added.
