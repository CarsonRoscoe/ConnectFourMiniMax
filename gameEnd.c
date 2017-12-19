#include "gameEnd.h"

inline int CheckForWin(*board) {
  int result = 0;
  result += CheckHorizontalLines(board);
  result += CheckVerticalLines(board);
  result += CheckDiagonalLines(board);
  if (result > 0) {
    return 1;
  } else if (result < 0) {
    return -1;
  } else {
    return 0;
  }
}

inline int CheckLine(int i1, int i2, int i3, int i4) {
  if (i1 == i2 && i2 == i3 && i3 == i4) {
    return i1;
  }
  return 0;
}

 inline int CheckHorizontalLines(*data) {
  int score = 0;

  for(int y = 0; y < 6; y++) {
      int offset = y * 7;
      for(int x = 0; x < 4; x++) {
          score += CheckLine(data[offset + x], data[offset + x + 1], data[offset + x + 2], data[offset + x + 3]);
      }
  }

  return score;
}

 inline int CheckVerticalLines(*data) {
  int score = 0;

  for(int x = 0; x < 7; x++) {
    for(int y = 0; y < 3; y++) {
      score += CheckLine(data[x + y * 7], data[x + ((y + 1) * 7)], data[x + ((y + 2) * 7)], data[x + ((y + 3) * 7)]);
    }
  }

  return score;
}

inline int CheckDiagonalLines(*data) {
  int score = 0;
  int y1 = 0;
  int y2 = 7;
  int y3 = 14;
  int y4 = 21;
  int y5 = 28;
  int y6 = 35;

  for(int x = 0; x < 7; x++) {
    if (x >= 3) {
      score += CheckLine(data[y1 + x - 3], data[y2 + x - 2], data[y3 + x - 1], data[y4 + x]);
      score += CheckLine(data[y2 + x - 3], data[y3 + x - 2], data[y4 + x - 1], data[y5 + x]);
      score += CheckLine(data[y3 + x - 3], data[y4 + x - 2], data[y5 + x - 1], data[y6 + x]);
    }
    if (x <= 3) {
      score += CheckLine(data[y4 + x], data[y3 + x + 1], data[y2 + x + 2], data[y1 + x + 3]);
      score += CheckLine(data[y5 + x], data[y4 + x + 1], data[y3 + x + 2], data[y2 + x + 3]);
      score += CheckLine(data[y6 + x], data[y5 + x + 1], data[y4 + x + 2], data[y3 + x + 3]);
    }
  }

  return score;
}
