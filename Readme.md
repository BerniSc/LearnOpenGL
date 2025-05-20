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
- Each clone [this repo](https://github.com/nothings/stb) has to be cloned directly into folder \<stb\>. Have issues with submodules and dont want to invest the Time.



## Fractal Project - Quick Notes
- Can change the iteration function and the number of iterations over fractal/shader/newton.fs.glsl<br>
TODO -> Put this in a seperate Shader? Or maybe just make it configurable via union?<br>
```glsl
    // Newton iteration
    for(int i = 0; i < 100; i++) {
        // Exit early if we already converge, Cool effect by checking against original root^ ^
        // if(checkRoot(z, r1) || checkRoot(z, r2) || checkRoot(z, r3))
        if(checkRoot(z, root1) || checkRoot(z, root2) || checkRoot(z, root3))
            break;
            
        // Newton: z - fx/fxdx
        // z = z - complexDiv(f(z), fPrime(z));

        // Different: z - (fxfxdxa)/(fxdx^2-fxfxdxdx)
        z = z - complexDiv(complexMul(f(z), fPrime(z)), \
                (complexSub(complexMul(fPrime(z), fPrime(z)), complexMul(f(z), fPrimePrime(z)))));
    }
```
- build via "make fractal"
- Have three roots that form the polynom. They start as unity roots.
    - Vim-Like modal approach: 
    - pressing "1", "2" or "3" will set view into "Change mode". _\<hjkl\>_ will then move the root a'la vim (h := left,j := down etc.)
    - pressing "Esc" will return to "Movement mode". Here _\<hjkl\>_ will move the screen.
    - pressing "a" will zoom in, pressing "d" will zoom out
    - pressing \<space\> will print the current set of selected roots
    - while in "Change mode" presets can be loaded. Currently these presets lie on "Arrow-Up" and "Arrow-Right". These can manually be set like this:
    ```cpp
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                root1.x = -1.2757;
                root1.y = -0.345167;
                root2.x = 0.998853;
                root2.y = -0.000889109;
                root3.x = 0.98774;
                root3.y = 2.02204;
            }

            if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                root1.x = 0.26132;
                root1.y = -0.0273117;
                root2.x = -0.710967;
                root2.y = 0.811377;
                root3.x = -0.476081;
                root3.y = -0.878705;
            }
    ```
    The style is pretty close to what can be taken from the console after printing the roots.
