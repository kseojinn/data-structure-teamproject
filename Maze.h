#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

// Location2D 구조체 정의
typedef struct {
    int row;
    int col;
} Location2D;

// 미로 맵 구조체 정의
typedef struct MazeMap {
    int width;
    int height;
    int **map;
    Location2D enter;
    Location2D exit;
} Maze;

// 큐 노드 구조체 (BFS용)
typedef struct QueueNode {
    Location2D pos;
    struct QueueNode* next;
} QueueNode;

// 큐 구조체 (BFS용)
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// 미로 관련 함수 선언
extern void init_maze(Maze* m, int w, int h);
extern void reset_maze(Maze* m);
extern void load_maze(Maze* m, char* fname);
extern void print_maze(Maze* m);

// 탐색 함수 선언
extern void search_maze_dfs(Maze* m);
extern void search_maze_bfs(Maze* m);

// 큐 관련 함수 선언 (BFS용)
extern void init_queue(Queue* q);
extern bool is_empty(Queue* q);
extern void enqueue(Queue* q, Location2D pos);
extern Location2D dequeue(Queue* q);
extern void clear_queue(Queue* q);

#endif
