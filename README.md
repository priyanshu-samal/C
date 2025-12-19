# C and C++ Learning Journey

Welcome to my programming repository. This "book" documents my journey of learning both C and C++ through practical projects. Each chapter dives into a specific project, explaining the concepts, code, and logic behind it.

## Index

### Part 1: C Projects
*   [**Chapter 1: The Obfuscator**](./C/obfuscator) - A simple tool to scramble text using bitwise operations.
*   [**Chapter 2: SDL Experiment**](./C/sdl) - An exploration of graphics programming and window management.
*   [**Chapter 3: Ping Pong**](#chapter-3-ping-pong) - A classic Pong game implemented from scratch.

### Part 2: C++ Projects
*   [**Chapter 4: Hello World**](./C++) - First steps into C++.

---

## Chapter 1: The Obfuscator

### Overview
The **Obfuscator** is a minimal utility designed to understand how data can be manipulated at the bit level. It takes standard input, flips the bits of every character, and prints the result to standard output. This simple transformation makes the text unreadable to humans but easily reversible by running it through the same program again.

### Concept
This program demonstrates two core concepts:
1.  **Standard I/O**: Reading from `stdin` and writing to `stdout` allows the program to be part of a pipeline (e.g., `echo "hello" | ./main`).
2.  **Bitwise NOT (`~`)**: The tilde operator inverts all bits of a number. (e.g., `00001111` becomes `11110000`).

### Data Flow
The following diagram illustrates how a single character flows through the program:

```mermaid
flowchart LR
    Input[Input Character 'A'] -->|stdin| Read(getchar)
    Read -->|int c| Trans[Transform function]
    Trans -->|~c| Inverted[Inverted Bits]
    Inverted -->|stdout| Write(putchar)
    Write --> Output[Output Character]
    
    style Input fill:#e1f5fe,stroke:#01579b
    style Output fill:#e1f5fe,stroke:#01579b
    style Trans fill:#fff9c4,stroke:#fbc02d
```

### Code Walkthrough
Let's look at `C/obfuscator/main.c` line by line.

```c
1: #include <stdio.h>
```
**Line 1**: We include the Standard Input/Output library. This gives us access to `getchar`, `putchar`, and `EOF`.

```c
2:  int transform(int c){
3:     return ~c;
4:  }
```
**Lines 2-4**: The `transform` function is the core logic.
*   It takes an integer `c` (a character code).
*   It returns `~c` (bitwise NOT of `c`).
*   **Why `int`?** In C, `getchar` returns an `int` to accommodate the `EOF` (End of File) marker, which is -1.

```c
6:  int main(){
7:     int c;
8:     while((c=getchar()) != EOF){
9:         putchar(transform(c));
10:     }
11:  }
```
**Line 6**: The entry point of the program.
**Line 7**: We declare a variable `c` to hold each character.
**Line 8**: This is a classic C idiom: `while((c=getchar()) != EOF)`.
1.  `getchar()` reads one character from the input.
2.  `(c = ...)` assigns that character to `c`.
3.  `... != EOF` checks if we have reached the end of the input stream.
4.  The loop continues as long as there is data to read.
**Line 9**: `putchar(transform(c))` calculates the obfuscated character and prints it immediately.

---

## Chapter 2: SDL Starter

### Overview
This project is an initial experiment with **SDL2** (Simple DirectMedia Layer). Unlike the console-based obfuscator, this program creates a graphical window. However, since it's a "starter" project, it also keeps a foot in the terminal world by running an ASCII spinner in the console while the window is open.

### Application Loop
Game engines and UI applications typically run in an infinite loop that handles events, updates state, and renders graphics.

```mermaid
stateDiagram-v2
    [*] --> Init: SDL_Init
    Init --> Window: Create Window
    
    state "Main Loop" as MainLoop {
        Poll: Handle Events (Input/Quit)
        Update: Update Spinner State
        Render: Draw to Console
        
        Poll --> Update: Events Processed
        Update --> Render: Logic Done
        Render --> Poll: Wait for Next Frame
    }
    
    Window --> MainLoop
    MainLoop --> CleanUp: Quit Event
    CleanUp --> [*]: SDL_Quit
```

### Key Components (`C/sdl/main.c`)

*   **SDL Initialization**: `SDL_Init(SDL_INIT_VIDEO)` sets up the video subsystem.
*   **Window Creation**: `SDL_CreateWindow` opens the actual OS window with the title "ASCII Spinner".
*   **The Event Loop**:
    ```c
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { ... }
    }
    ```
    This non-blocking loop checks for user actions (like clicking the 'X' button) to ensure the application stays responsive.
*   **The Spinner**: The code manually refreshes the console output using ANSI escape codes (`\033[2J\033[H`) to clear the screen and print the next frame of the animation, creating a simple visual effect in the terminal alongside the empty SDL window.

---

## Chapter 3: Ping Pong

### Overview
**Pong in C using SDL2** is a classic Pong game implemented from scratch. This project was built to deeply understand low-level memory manipulation, pointers, real-time game loops, collision physics, and state-driven architecture.

![Gameplay Screenshot](./C/Ping%20Pong/pingpong.png)

### Features
*   **🎮 Two-player Pong** (keyboard controlled)
*   **⏯️ Start / Pause / Resume** game
*   **🧠 Constant-speed ball physics** (no speed bugs)
*   **🧱 Proper collision handling** (walls & paddles)
*   **📝 Text rendering** using SDL2_ttf
*   **🧩 Clean game-state architecture**

### Controls
| Key | Action |
| :--- | :--- |
| **W / S** | Player 1 paddle up / down |
| **↑ / ↓** | Player 2 paddle up / down |
| **SPACE** | Start / Pause / Resume |
| **X** | Restart game (reset score) |
| **ESC** | Quit |

### High-Level Architecture
The game is structured into three core phases, executed every frame. This separation ensures input doesn’t directly affect rendering and physics doesn’t run when paused.

```mermaid
flowchart TD
    Input[Input Handling] --> Update[Game Logic / Physics]
    Update --> Render[Rendering]
    Render --> Input
```

### Game State System
The game uses a state machine to control behavior:

```mermaid
stateDiagram-v2
    WAIT --> PLAY : Press SPACE
    PLAY --> PAUSE : Press SPACE
    PAUSE --> PLAY : Press SPACE
    PLAY --> WAIT : Score Event

    state "GAME_WAIT" as WAIT
    state "GAME_PLAY" as PLAY
    state "GAME_PAUSE" as PAUSE
```

*   **GAME_WAIT**: Ball is reset, waiting to start.
*   **GAME_PLAY**: Physics and movement active.
*   **GAME_PAUSE**: Screen frozen, input still active.

### Technical Deep Dive
#### Memory & Pointer Design
This project intentionally uses **pointers** everywhere movement or state must persist.

```c
void move_paddle(SDL_Rect *p, int dy)
```

Passing by **value** would modify a copy. Passing a **pointer** modifies the actual object in memory. This is how real engines work at a low level: nothing "moves" on screen; numbers in memory change, and SDL draws those numbers.

#### Physics (Ball Movement)
The ball uses float-based velocity vectors to prevent speed bugs and inconsistent gameplay.

```mermaid
flowchart LR
    Collision --> AngleCalc
    AngleCalc --> Normalize
    Normalize --> ConstantSpeed
```

1.  **Angle Calculation**: Calculated based on where the ball hits the paddle.
2.  **Normalization**: The velocity vector is normalized.
3.  **Constant Speed**: Speed magnitude is restored to a constant value.

### File Structure
```
Ping Pong/
├── main.c        # Entire game logic
├── arial.ttf     # Font used for score rendering
├── pingpong.png  # Gameplay screenshot
└── pingpong.exe  # Generated buffer
```

### How to Build & Run (Step-by-Step)
**Windows (MSYS2 – MinGW64)**

1.  **Install dependencies**
    ```bash
    pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
    ```

2.  **Navigate to project folder**
    ```bash
    cd "C/Ping Pong"
    ```

3.  **Compile**
    ```bash
    gcc main.c -o pingpong $(pkg-config --cflags --libs sdl2 SDL2_ttf) -lm
    ```

4.  **Run**
    ```bash
    ./pingpong
    ```

> [!WARNING]
> Ensure `arial.ttf` is in the same folder as the executable, otherwise the game may not load the font.

---

## Chapter 4: Hello World (C++)

### Overview
This is the classic entry point for any language. In C++, we use the Standard Template Library (STL) streams for output.

### Code Walkthrough (`C++/main.cpp`)

```cpp
1: #include <iostream>
2: using namespace std;
3: int main(){
4:     cout << "Hello World";
5:     return 0;
6: }
```

*   `#include <iostream>`: Includes the Input/Output Stream library, replacing C's `stdio.h`.
*   `using namespace std;`: Allows us to use `cout` instead of `std::cout`.
*   `cout <<`: "Character Output". The `<<` operator directs the string "Hello World" to the output stream.

---
*Happy Coding!*
