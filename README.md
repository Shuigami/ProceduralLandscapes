# Procedural OpenGL Landscape

A first-person 3D landscape you can fly through, built from the ground up with OpenGL 4.5. The world is not authored in a scene editor: it is generated as you move, from Perlin noise, a handful of meshes, and a lighting model that tries to feel like open air rather than a lab demo.

This is a practical graphics project (TP OpenGL). The interesting part is not a game loop or a content pipeline. It is that most of the usual 3D stack is written here: vectors and matrices, a shader program loader, a camera, chunked terrain, texture blending, fog, and a small object system.

## What it feels like

You spawn above the ground and look out over rolling hills. Low elevations are grass, mid-slopes turn to rock, and the highest ridges pick up a noisy snow line so the peaks never look stamped. Sparse trees sit on the lower slopes. Soft, blocky clouds hang in the sky. Distance fades into a bright blue haze that matches the clear color, so the horizon does not cut off as a hard edge.

The map is effectively unbounded. Chunks appear around the camera and fall away behind it. You can walk, fly, and look around with a captured mouse. Frame rate is printed to the console so you can feel the cost of render distance as you raise or lower it.

## How the world is made

Terrain height comes from octave Perlin noise. Several frequencies are summed, then a shaping function (`x⁶ × 2000` in the current scene) stretches the distribution into something more mountainous than a gentle meadow. The seed, scale, octaves, persistence, and lacunarity are all parameters on `Map`; changing them changes the character of the land without touching geometry code.

The surface is streamed in square chunks. Each chunk is a height-displaced grid whose normals are rebuilt from the triangles, so lighting follows the slopes. Only chunks inside the current render distance stay in memory. That distance can be changed at runtime with `+` / `-`.

Height also drives material. The fragment shader blends grass, rock, and snow with `smoothstep` bands, then jitters the snow threshold with a cheap 2D noise so the white cap is irregular instead of a contour line. Trees (OBJ trunk and leaves) spawn rarely on grass-level faces. Clouds are clusters of cubes with alpha, sorted back-to-front so transparency does not paint over itself.

## How it is drawn

The renderer is a single GLSL 4.50 program. The vertex shader puts positions into view space, transforms normals, and measures fog distance. The fragment shader chooses a material (solid color, one texture, or the three-way terrain blend), then applies a directional sun: warm ambient, Lambert diffuse, and a specular term. Distance fog is mixed last so far geometry dissolves into the sky.

Opaque objects draw first, then a reference cube at the origin, then transparent clouds with the depth mask off. Back-face culling and depth testing stay on for solid geometry. Projection is a frustum rebuilt on resize; the view matrix is a look-at built from the camera each frame.

A lot of the graphics math is local: `Vector3` / `Vector4` and `Matrix4` implement translation, rotation, scale, look-at, and frustum. GLM is used only in a few places (mouse look, triangle normals). Shaders, textures, and OBJ files are loaded from disk; CMake copies `shaders/` and `textures/` next to the binary.

## Layout

| Path | Role |
|------|------|
| `src/main.cpp` | Window loop, map setup, draw order |
| `src/map.cpp` | Noise, chunks, trees, clouds, fog parameters |
| `src/object.cpp` | Meshes, OBJ loader, textures, transforms |
| `src/camera.cpp` | WASD / mouse look, render-distance keys |
| `src/settings.cpp` | GLFW / GLEW / GL init, uniforms, draw |
| `src/program.cpp` | Shader compile and link |
| `src/matrix.cpp`, `src/vector.cpp` | Homegrown linear algebra |
| `shaders/` | Vertex and fragment programs |
| `textures/` | Grass, rock, snow, wood, leaf, palette colors |
| `models/` | Tree parts (and unused sample meshes) |
| `libs/` | Vendored GLEW, GLFW, GLM for Windows |

## Controls

| Input | Action |
|-------|--------|
| Mouse | Look around |
| `W` `A` `S` `D` | Move in the look direction |
| `Space` / `Ctrl` | Up / down |
| `+` / `-` | Raise or lower render distance (1–10) |

The cursor is captured. Close the window to quit. FPS prints once a second.

## Building

C++17, OpenGL 4.5, CMake 3.10+.

**Windows** — GLEW and GLFW are already under `libs/`. CMake copies `glew32.dll` and `glfw3.dll` into the build directory.

```bash
cmake -B build
cmake --build build
```

Run the `opengl` target from a working directory that can see `shaders/`, `textures/`, and `models/` (CMake copies the first two into the build tree; models are still read from the project root).

**Linux** — install system packages for OpenGL, GLEW, GLFW, and GLM, then the same CMake commands. The CMake file looks them up with `find_package`.

## What this project is practicing

- A programmable pipeline instead of fixed-function OpenGL
- Camera, projection, and lighting as matrices and uniforms you own
- Procedural content that stays coherent across chunk boundaries
- Height-based materials and atmospheric fog as image-making tools, not afterthoughts
- Mixing generated meshes with loaded OBJ assets
- Correct-enough transparency: sort, then disable depth writes

The scene is small on purpose. The value is that you can read from `main.cpp` down to a vertex and still know why a hill looks like a hill.
