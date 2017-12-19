#include "ai.h"
#include <stdio.h>
#include "openCLTest.h"
#include <stdlib.h>
#include "draw.h"
#include "gameEnd.h"

int aiPlayer = 0;
int maximumDepth = 0;
int shallowDepth = 3;

inline int ScoreLine(int i1, int i2, int i3, int i4) {
  int score = 0;

  int xs = 0;
  int os = 0;

  if (i1 < 0) {
    os++;
  } else if (i1 > 0) {
    xs++;
  }

  if (i2 < 0) {
    os++;
  } else if (i2 > 0) {
    xs++;
  }

  if (i3 < 0) {
    os++;
  } else if (i3 > 0) {
    xs++;
  }

  if (i4 < 0) {
    os++;
  } else if (i4 > 0) {
    xs++;
  }

  score = (xs * xs * xs) - (os * os * os);

  if (score == 1 || score == 8 || score == 27 || score == -1 || score == -8 || score == -27) {
    return score;
  }
  if (score == 64 || score == -64) {
    return score * 100000;
  }
  return 0;
}

 int ScoreHorizontalLines(*data, int startOffset) {
  int score = 0;

  for(int y = 0; y < 6; y++) {
      int offset = startOffset + y * 7;
      for(int x = 0; x < 4; x++) {
          score += ScoreLine(data[offset + x], data[offset + x + 1], data[offset + x + 2], data[offset + x + 3]);
      }
  }

  return score;
}

 int ScoreVerticalLines(*data, int startOffset) {
  int score = 0;

  for(int x = 0; x < 7; x++) {
    int offset = startOffset + x;
    for(int y = 0; y < 3; y++) {
      score += ScoreLine(data[offset + y * 7], data[offset + ((y + 1) * 7)], data[offset + ((y + 2) * 7)], data[offset + ((y + 3) * 7)]);
    }
  }

  return score;
}

int ScoreObtuseDiagonalLines(*data, int offset) {
  int score = 0;
  int y1 = 0;
  int y2 = 7;
  int y3 = 14;
  int y4 = 21;
  int y5 = 28;
  int y6 = 35;

  for(int x = 0; x < 7; x++) {
    if (x >= 3) {
      score += ScoreLine(data[offset + y1 + x - 3], data[offset + y2 + x - 2], data[offset + y3 + x - 1], data[offset + y4 + x]);
      score += ScoreLine(data[offset + y2 + x - 3], data[offset + y3 + x - 2], data[offset + y4 + x - 1], data[offset + y5 + x]);
      score += ScoreLine(data[offset + y3 + x - 3], data[offset + y4 + x - 2], data[offset + y5 + x - 1], data[offset + y6 + x]);
    }
    if (x <= 3) {
      score += ScoreLine(data[offset + y4 + x], data[offset + y3 + x + 1], data[offset + y2 + x + 2], data[offset + y1 + x + 3]);
      score += ScoreLine(data[offset + y5 + x], data[offset + y4 + x + 1], data[offset + y3 + x + 2], data[offset + y2 + x + 3]);
      score += ScoreLine(data[offset + y6 + x], data[offset + y5 + x + 1], data[offset + y4 + x + 2], data[offset + y3 + x + 3]);
    }
  }

  return score;
}

int* evaluator(int* data, int boardCount) {
    int* scores = malloc(boardCount * sizeof(int));
    for(int i = 0; i < boardCount; i++) {
      int score = 0;
      int offset =  i * 7 * 6;
      score += ScoreHorizontalLines(data, offset);
      score += ScoreVerticalLines(data, offset);
      score += ScoreObtuseDiagonalLines(data, offset);
      score += data[offset + (5 * 7) + 3] * 50;
      score += data[offset + (4 * 7) + 3] * 25;
      score += data[offset + (3 * 7) + 3] * 15;
      score += data[offset + (2 * 7) + 3] * 10;
      if (score > 5000000) {
        score = 5500000;
      } else if (score < -5000000) {
        score = -5500000;
      }
      scores[i] = score;
    }
    return scores;
}


void initializeAI(int val, int maxDepth) {
  aiPlayer = val;
  maximumDepth = maxDepth;
  srand ( 10 );
}

//BoardData for 1 board
int* pickMove(int* boardData) {
  int score = 0;
  int* result = aiPlayer > 0 ? max(boardData, &score, 1) : min(boardData, &score, 1);
  printf("AI Decision Score: %d\n", score);
  return result;
}

int* max(int* boardData, int* score, int depth) {
  int bestMinScore = -200000000;
  int* bestMinBoard = malloc(7 * 6 * sizeof(int));
  int subBoardCount;

  if (depth < maximumDepth) {
    //Check if we're a win
    int myScore = evaluator(boardData, 1)[0];

    if (myScore < -5000000) {
      bestMinScore = myScore;
      boardCopy(boardData, bestMinBoard, 0, 0);
    } else {
      int* subBoards = determineSubBoards(boardData, &subBoardCount, 1);
      //If we have valid moves to make
      if (subBoardCount > 0) {
        int* subBoardScores = evaluator(subBoards, subBoardCount);
        int subBoardScores2[subBoardCount];
        int worstScore = 200000000;
        int averageScore = 0;
        for(int i = 0; i < subBoardCount; i++) {
          if (subBoardScores[i] < worstScore) {
            worstScore = subBoardScores[i];
          }
          averageScore += subBoardScores[i];
        }
        averageScore /= subBoardCount;
        //For all boards we think are better than average, min it, get it's real answer
        for(int i = 0; i < subBoardCount; i++) {
          int currentScore = subBoardScores[i];
          if ((depth < 3) || (subBoardCount > 2 && ((depth < 4 && currentScore != worstScore) || (currentScore >= averageScore))) || subBoardCount <= 2) {
            int actualScore = 0;
            int* boardDataToMin = malloc(7 * 6 * sizeof(int));
            int* minnedBoard = min(subBoards + (i * 6 * 7), &actualScore, depth + 1);
            if (actualScore >= bestMinScore) {
              bestMinScore = actualScore;
              boardCopy(subBoards, bestMinBoard, i, 0);
            }
            subBoardScores2[i] = actualScore;
            free(minnedBoard);
            free(boardDataToMin);
          }
        }
        free(subBoardScores);
      }
      free(subBoards);
    }
  } else {
    bestMinScore = evaluator(boardData, 1)[0];
    boardCopy(boardData, bestMinBoard, 0, 0);
  }

  (*score) = bestMinScore;

  return bestMinBoard;
}

int* min(int* boardData, int* score, int depth) {
  int bestMaxScore = 200000000;
  int* bestMaxBoard = malloc(7 * 6 * sizeof(int));
  int subBoardCount;
  if (depth < maximumDepth) {
    //Determine all possible moves

    //Check if we're a win
    int myScore = evaluator(boardData, 1)[0];

    if (myScore > 5000000) {
      bestMaxScore = myScore;
      boardCopy(boardData, bestMaxBoard, 0, 0);
    } else {
      int* subBoards = determineSubBoards(boardData, &subBoardCount, -1);
      //If we have valid moves to make
      if (subBoardCount > 0) {
        int* subBoardScores = evaluator(subBoards, subBoardCount);
        int subBoardScores2[subBoardCount];
        int worstScore = -200000000;
        int averageScore = 0;
        for(int i = 0; i < subBoardCount; i++) {
          if (subBoardScores[i] > worstScore) {
            worstScore = subBoardScores[i];
          }
          averageScore += subBoardScores[i];
        }
        averageScore /= subBoardCount;

        for(int i = 0; i < subBoardCount; i++) {
          int currentScore = subBoardScores[i];
          if ((depth < 3) || (subBoardCount > 2 && ((depth < 4 && currentScore != worstScore) || (currentScore <= averageScore))) || subBoardCount <= 2) {
            int actualScore = 0;
            int* boardDataToMax = malloc(7 * 6 * sizeof(int));
            int* maxedBoard = max(subBoards + (i * 6 * 7), &actualScore, depth + 1);

            if (actualScore < bestMaxScore) {
              bestMaxScore = actualScore;
              boardCopy(subBoards, bestMaxBoard, i, 0);
            }
            subBoardScores2[i] = actualScore;
              free(maxedBoard);
            }
          }
          free(subBoardScores);
        }
      free(subBoards);
    }
  } else {
    bestMaxScore = evaluator(boardData, 1)[0];
    boardCopy(boardData, bestMaxBoard, 0, 0);
  }

  (*score) = bestMaxScore;
  return bestMaxBoard;
}

inline void boardCopy(int* source, int* destination, int sourceBoardOffset, int destinationBoardOffset) {
  int destinationOffset = destinationBoardOffset * 7 * 6;
  int sourceOffset = sourceBoardOffset * 7 * 6;
  for(int y = 0; y < 6; y++) {
    for(int x = 0; x < 7; x++) {
      destination[destinationOffset + y * 7 + x] = source[sourceOffset + y * 7 + x];
    }
  }
}

int* createBoardIfPossible(int* boardData, int x, int* counter, int piece) {
  int* result = malloc(7 * 6 * sizeof(int));
  int complete = -1;

  //If we can drop in this column
  for(int y = 5; y >= 0; y--) {
    if (boardData[y * 7 + x] == 0) {
      boardCopy(boardData, result, 0, 0);
      result[y * 7 + x] = piece;
      (*counter)++;
      complete = 0;
      break;
    }
  }
  if (complete != 0) {
    free(result);
    result = NULL;
  }
  return result;
}

//BoardData for 1 board
int* determineSubBoards(int* boardData, int* boardCount, int piece) {
  int subboards = 0;
  int curCounter = 0;
  int* result;
  int* boards[7];

  for(int i = 0; i < 7; i++) {
    boards[i] = createBoardIfPossible(boardData, i, &subboards, piece);
  }

  result = malloc(7*6*subboards*sizeof(int));

  for(int i = 0; i < 7; i++) {
    if (boards[i] != NULL) {
      boardCopy(boards[i], result, 0, curCounter);
      curCounter++;
      free(boards[i]);
    }
  }

  *boardCount = subboards;
  return result;
}
