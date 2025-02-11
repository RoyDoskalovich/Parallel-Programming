# Target executable name
TARGET = part_III.out

# Source files
SRCS = main.c BFS/parallelBFS.c LevelQueue/LevelQueue.c ThreadPool/ThreadPool.c ThreadPool/TaskQueue.c SyncGraph/graph.c

# Compiler and flags
CC = gcc
CFLAGS = -g


# Default target builds the executable
$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o  $@ -pthread

# Clean target removes the executable
clean:
	rm -f $(TARGET)

