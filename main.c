#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>

// 미로 상태 백업 및 복원 함수
void backup_maze(Maze* original, Maze* backup) {
    init_maze(backup, original->width, original->height);
    backup->enter = original->enter;
    backup->exit = original->exit;
    
    for (int i = 0; i < original->height; i++) {
        for (int j = 0; j < original->width; j++) {
            backup->map[i][j] = original->map[i][j];
        }
    }
}

void restore_maze(Maze* original, Maze* backup) {
    for (int i = 0; i < original->height; i++) {
        for (int j = 0; j < original->width; j++) {
            original->map[i][j] = backup->map[i][j];
        }
    }
}

int main() {
    Maze maze, backup_maze_struct;
    
    printf("==============================================\n");
    printf("   미로 탐색 프로그램 - DFS vs BFS 비교\n");
    printf("==============================================\n\n");
    
    // 미로 파일 로드
    load_maze(&maze, "maze.txt");
    
    // 원본 미로 백업
    backup_maze(&maze, &backup_maze_struct);
    
    // 초기 미로 상태 출력
    printf("=== 초기 미로 상태 ===");
    print_maze(&maze);
    
    // ==================
    // DFS 탐색
    // ==================
    printf("\n=== DFS (깊이 우선 탐색) 실행 ===\n");
    search_maze_dfs(&maze);
    
    printf("=== DFS 탐색 결과 ===");
    print_maze(&maze);
    
    // 미로 상태 복원
    restore_maze(&maze, &backup_maze_struct);
    
    // ==================
    // BFS 탐색
    // ==================
    printf("\n=== BFS (너비 우선 탐색) 실행 ===\n");
    search_maze_bfs(&maze);
    
    printf("=== BFS 탐색 결과 ===");
    print_maze(&maze);
    
    // ==================
    // 비교 분석
    // ==================
    printf("\n=== 탐색 방법 비교 ===\n");
    printf("DFS (깊이 우선 탐색):\n");
    printf("  - 표시: ○ (성공 경로), × (막다른 길)\n");
    printf("  - 특징: 한 방향으로 끝까지 탐색 후 백트래킹\n");
    printf("  - 장점: 메모리 사용량 적음, 구현 간단\n");
    printf("  - 단점: 최단 경로 보장 안됨\n\n");
    
    printf("BFS (너비 우선 탐색):\n");
    printf("  - 표시: ● (최단 경로)\n");
    printf("  - 특징: 시작점에서 같은 거리의 모든 점을 동시에 탐색\n");
    printf("  - 장점: 최단 경로 보장\n");
    printf("  - 단점: 메모리 사용량 많음\n\n");
    
    // 메모리 해제
    reset_maze(&maze);
    reset_maze(&backup_maze_struct);
    
    return 0;
}
