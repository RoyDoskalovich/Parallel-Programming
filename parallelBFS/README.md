# Parallel BFS with Thread Pool

## Overview
This subdirectory contains a **parallel implementation** of **Breadth-First Search (BFS)** using a **Task Queue and Thread Pool** for efficient multi-threading.

### Key Components
- **Task Queue (`TaskQueue.c/h`)**: Manages tasks for worker threads.
- **Thread Pool (`ThreadPool.c/h`)**: Dynamically spawns threads to process BFS levels in parallel.
- **Graph Structure (`graph.c/h`)**: Stores adjacency lists and visit tracking.
- **Level Queue (`LevelQueue.c/h`)**: Synchronizes nodes across BFS levels.

### How It Works
1. **Graph Creation**: Build a graph from user input.
2. **Parallel BFS Execution**:
   - Uses a **task queue** to distribute work among threads.
   - Each thread processes a chunk of the BFS traversal.
   - Mutex locks ensure thread-safe operations.
3. **Shortest Path Matrix**: Outputs a matrix representing the shortest distances between nodes.

## Usage
1. Compile using:
   ```sh
   make
   ```
2. Run the program:
   ```sh
   ./parallelBFS.out
   ```
3. Enter the number of vertices and edges, then define edges.


