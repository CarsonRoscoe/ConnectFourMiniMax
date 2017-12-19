#include "draw.h"
#include "stdio.h"

void drawBoard(int* boardData) {
  int y, x;
  if (boardData != NULL) {
    printf("::Drawing Board::\n");
    for(y = 0; y < 6; y++) {
      for (x = 0; x < 7; x++) {
        switch (boardData[y * 7 + x]) {
          case 0:
            printf(" - ");
            break;
          case 1:
            printf(" X ");
            break;
          case -1:
            printf(" O ");
            break;
        }
      }
      printf("\n");
    }
    fflush(stdout);
  } else {
    printf("::Drawing Board::Failed\n");
  }
}

void drawBoards(int* boards, int boardCount) {
  if (boards != NULL) {
    printf("::Drawing Boards::\n");
    for(int bc = 0; bc < boardCount; bc++) {
      int bcOffset = bc * 7 * 6;
      for(int y = 0; y < 6; y++) {
        int yOffset = y * 7;
        for(int x = 0; x < 7; x++) {
          printf("%d", boards[bcOffset + yOffset + x]);
        }
        printf("\n");
      }
      printf("\n");
    }
  } else {
    printf("::Drawing Boards::Failed\n");
  }
}
