### Simple game using Raylib.

### Local Development Setup

1. **Install Emscripten**
   
   Follow the official installation guide: https://emscripten.org/docs/getting_started/downloads.html

2. **Clone raylib:**
```bash
   mkdir -p external
   git clone https://github.com/raysan5/raylib.git external/raylib
```

3. **Build raylib for web:**
```bash
   cd external/raylib/src
   emmake make PLATFORM=PLATFORM_WEB -j4
   cd ../../..
```

4. **Build and run the game:**
```bash
   make
   make run
```
Open your browser and navigate to: http://localhost:8080/game.html
