#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 방향 배열: 상, 하, 좌, 우
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// (1) 미로 초기화 함수
void init_maze(Maze* m, int w, int h) {
    m->width = w;
    m->height = h;
    
    // 2차원 배열 동적 할당
    m->map = (int**)malloc(h * sizeof(int*));
    for (int i = 0; i < h; i++) {
        m->map[i] = (int*)malloc(w * sizeof(int));
    }
    
    // 배열 초기화 (모든 값을 0으로)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            m->map[i][j] = 0;
        }
    }
    
    // 입구와 출구 초기화
    m->enter.row = 0;
    m->enter.col = 0;
    m->exit.row = 0;
    m->exit.col = 0;
}

// (1) 미로 메모리 해제 함수
void reset_maze(Maze* m) {
    if (m->map != NULL) {
        for (int i = 0; i < m->height; i++) {
            if (m->map[i] != NULL) {
                free(m->map[i]);
            }
        }
        free(m->map);
        m->map = NULL;
    }
    m->width = 0;
    m->height = 0;
}

// (2) 미로 파일 로드 함수
void load_maze(Maze* m, char* fname) {
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다: %s\n", fname);
        return;
    }
    
    int w, h;
    // 첫 번째 줄에서 가로, 세로 크기 읽기
    fscanf(file, "%d %d", &w, &h);
    
    // 미로 초기화
    init_maze(m, w, h);
    
    // 미로 데이터 읽기
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            fscanf(file, "%d", &m->map[i][j]);
            
            // 입구와 출구 위치 저장
            if (m->map[i][j] == 5) {
                m->enter.row = i;
                m->enter.col = j;
            } else if (m->map[i][j] == 9) {
                m->exit.row = i;
                m->exit.col = j;
            }
        }
    }
    
    fclose(file);
    printf("미로 파일 '%s' 로드 완료\n", fname);
}

// (3) 미로 출력 함수
void print_maze(Maze* m) {
    printf("\n미로 맵 (크기: %d x %d)\n", m->width, m->height);
    printf("입구: (%d,%d), 출구: (%d,%d)\n", m->enter.row, m->enter.col, m->exit.row, m->exit.col);
    
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            switch (m->map[i][j]) {
                case 0: printf("■ "); break;  // 벽
                case 1: printf("  "); break;  // 길
                case 2: printf("○ "); break;  // DFS 방문 경로
                case 3: printf("× "); break;  // DFS 막다른 길
                case 4: printf("● "); break;  // BFS 방문 경로
                case 5: printf("S "); break;  // 시작점
                case 9: printf("E "); break;  // 출구
                default: printf("%d ", m->map[i][j]); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

// ================================
// DFS (깊이 우선 탐색) 구현
// ================================

// 미로 원본 상태 복사
void copy_maze(Maze* dest, Maze* src) {
    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            dest->map[i][j] = src->map[i][j];
        }
    }
}

// DFS 재귀 탐색 함수
bool dfs_search(Maze* m, int x, int y) {
    // 출구에 도달한 경우
    if (x == m->exit.row && y == m->exit.col) {
        return true;
    }
    
    // 현재 위치를 방문 표시 (경로를 2로 표시)
    m->map[x][y] = 2;
    
    // 4방향으로 탐색
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // 경계 검사
        if (nx >= 0 && nx < m->height && ny >= 0 && ny < m->width) {
            // 갈 수 있는 길(1) 또는 출구(9)인 경우
            if (m->map[nx][ny] == 1 || m->map[nx][ny] == 9) {
                if (dfs_search(m, nx, ny)) {
                    return true; // 출구를 찾은 경우
                }
            }
        }
    }
    
    // 백트래킹: 이 경로로는 출구를 찾을 수 없으므로 되돌아감
    // 방문했지만 막다른 길인 경우 3으로 표시
    if (!(x == m->enter.row && y == m->enter.col)) {
        m->map[x][y] = 3; // 방문했지만 막다른 길
    }
    
    return false;
}

// DFS 미로 탐색 메인 함수
void search_maze_dfs(Maze* m) {
    printf("=== DFS (깊이 우선 탐색) 시작 ===\n");
    
    // 입구에서 탐색 시작
    if (dfs_search(m, m->enter.row, m->enter.col)) {
        printf("DFS로 출구를 찾았습니다!\n");
        
        // 입구 위치 복원
        m->map[m->enter.row][m->enter.col] = 5;
        
        // 출구 위치 복원  
        m->map[m->exit.row][m->exit.col] = 9;
    } else {
        printf("DFS로 출구를 찾을 수 없습니다.\n");
    }
}

// ================================
// BFS (너비 우선 탐색) 구현
// ================================

// 큐 초기화
void init_queue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

// 큐가 비어있는지 확인
bool is_empty(Queue* q) {
    return q->front == NULL;
}

// 큐에 요소 추가
void enqueue(Queue* q, Location2D pos) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->pos = pos;
    newNode->next = NULL;
    
    if (is_empty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// 큐에서 요소 제거
Location2D dequeue(Queue* q) {
    Location2D pos = {-1, -1}; // 에러 값
    
    if (!is_empty(q)) {
        QueueNode* temp = q->front;
        pos = temp->pos;
        q->front = q->front->next;
        
        if (q->front == NULL) {
            q->rear = NULL;
        }
        
        free(temp);
    }
    
    return pos;
}

// 큐 메모리 해제
void clear_queue(Queue* q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
}

// BFS 탐색 함수
bool bfs_search(Maze* m) {
    Queue q;
    init_queue(&q);
    
    // 부모 노드 추적을 위한 2차원 배열
    Location2D** parent = (Location2D**)malloc(m->height * sizeof(Location2D*));
    for (int i = 0; i < m->height; i++) {
        parent[i] = (Location2D*)malloc(m->width * sizeof(Location2D));
        for (int j = 0; j < m->width; j++) {
            parent[i][j].row = -1;
            parent[i][j].col = -1;
        }
    }
    
    // 방문 여부 체크 배열
    bool** visited = (bool**)malloc(m->height * sizeof(bool*));
    for (int i = 0; i < m->height; i++) {
        visited[i] = (bool*)malloc(m->width * sizeof(bool));
        for (int j = 0; j < m->width; j++) {
            visited[i][j] = false;
        }
    }
    
    // 시작점을 큐에 추가
    enqueue(&q, m->enter);
    visited[m->enter.row][m->enter.col] = true;
    
    bool found = false;
    
    while (!is_empty(&q) && !found) {
        Location2D current = dequeue(&q);
        
        // 출구에 도달했는지 확인
        if (current.row == m->exit.row && current.col == m->exit.col) {
            found = true;
            break;
        }
        
        // 4방향으로 탐색
        for (int i = 0; i < 4; i++) {
            int nx = current.row + dx[i];
            int ny = current.col + dy[i];
            
            // 경계 검사
            if (nx >= 0 && nx < m->height && ny >= 0 && ny < m->width) {
                // 갈 수 있는 길이고 방문하지 않은 경우
                if ((m->map[nx][ny] == 1 || m->map[nx][ny] == 9) && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    parent[nx][ny] = current;
                    
                    Location2D next = {nx, ny};
                    enqueue(&q, next);
                }
            }
        }
    }
    
    // 경로 역추적 및 표시
    if (found) {
        Location2D current = m->exit;
        
        // 출구에서 입구까지 역추적하며 경로 표시
        while (!(current.row == m->enter.row && current.col == m->enter.col)) {
            if (!(current.row == m->exit.row && current.col == m->exit.col)) {
                m->map[current.row][current.col] = 4; // BFS 경로는 4로 표시
            }
            current = parent[current.row][current.col];
        }
    }
    
    // 메모리 해제
    for (int i = 0; i < m->height; i++) {
        free(parent[i]);
        free(visited[i]);
    }
    free(parent);
    free(visited);
    clear_queue(&q);
    
    return found;
}

// BFS 미로 탐색 메인 함수
void search_maze_bfs(Maze* m) {
    printf("=== BFS (너비 우선 탐색) 시작 ===\n");
    
    if (bfs_search(m)) {
        printf("BFS로 출구를 찾았습니다!\n");
        
        // 입구와 출구 위치 복원
        m->map[m->enter.row][m->enter.col] = 5;
        m->map[m->exit.row][m->exit.col] = 9;
    } else {
        printf("BFS로 출구를 찾을 수 없습니다.\n");
    }
}
