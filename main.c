//
//  main.c
//
//
//  Created by Carson Roscoe on 2017-11-02.
//

#include "main.h"
#include <stdio.h>
#include "ai.h"
#include <stdlib.h>
#include "draw.h"
#include "gameEnd.h"

int main(int argc, char** args) {
    //Create start board
    int* boardData = (int*)malloc(BOARD_HEIGHT*BOARD_WIDTH*sizeof(int)); //Free plz
    int y, x;
    for(y = 0; y < BOARD_HEIGHT; y++) {
        int yOffset = y * BOARD_WIDTH;
        for(x = 0; x < BOARD_WIDTH; x++) {
           boardData[yOffset + x] = 0;
        }
    }

    int playerCounter;
    int playerPiece;

    printf("Enter if the AI should go first [1] or second [2]\n\n");
    char c = fgetc(stdin);
    if (c == '1') {
      initializeAI(1, 13);
      playerPiece = -1;
    } else if (c == '2') {
      initializeAI(-1, 13);
      playerPiece = 1;
    }

    int keepPlaying = 1;
    int turn = 1;

    do {
      int* newBoard = NULL;

      //If it's the first players turn and AI is player 1 OR 2nd players turn and AI is player 2
      if ( (turn % 2 == 1 && c == '1') || (turn % 2 == 0 && c == '2')) {
        printf("##AI 1 turn##\n");
        newBoard = pickMove(boardData);
      } else {
        do {
          printf("##Enter your move [1-7]##\n");
          int column = 0;

          while (column < 1 || column > 7) {
            fscanf(stdin, "%d", &column );
          }

          fflush(stdout);
          newBoard = createBoardIfPossible(boardData, column-1, &playerCounter, playerPiece );
        } while (newBoard == NULL);
      }

      free(boardData);
      boardData = newBoard;
      drawBoard(boardData);

      int gameResult = CheckForWin(boardData);

      if (gameResult == 1) {
        printf("\nPlayer One Won!\n");
        keepPlaying = 0;
      } else if (gameResult == -1) {
        printf("\nPlayer Two Won!\n");
        keepPlaying = 0;
      } else if (turn == 41) {
        printf("\nDraw!\n");
        keepPlaying = 0;
      }

      turn++;
    } while (keepPlaying);
}
