__kernel void evaluator(__global int* data,
                        __global int* group_result) {
    int score = 0;

    uint local_addr = get_local_id(0);

    int startOffset = local_addr * 6 * 7;

    score += ScoreHorizontalLines(data, startOffset);
    score += ScoreVerticalLines(data, startOffset);
    score += ScoreObtuseDiagonalLines(data, startOffset);

    group_result[local_addr] = score;
}

inline int ScoreObtuseDiagonalLines(*data, int offset) {
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

inline int ScoreVerticalLines(*data, int startOffset) {
  int score = 0;

  for(int x = 0; x < 7; x++) {
    int offset = startOffset + x;
    for(int y = 0; y < 3; y++) {
      score += ScoreLine(data[offset + y * 7], data[offset + ((y + 1) * 7)], data[offset + ((y + 2) * 7)], data[offset + ((y + 3) * 7)]);
    }
  }

  return score;
}

inline int ScoreHorizontalLines(*data, int startOffset) {
  int score = 0;

  for(int y = 0; y < 6; y++) {
      int offset = startOffset + y * 7;
      for(int x = 0; x < 4; x++) {
          score += ScoreLine(data[offset + x], data[offset + x + 1], data[offset + x + 2], data[offset + x + 3]);
      }
  }

  return score;
}

inline int ScoreLine(int i1, int i2, int i3, int i4) {
  int score = 0;

  score += DeterminePair(i1, i2);
  score += DeterminePair(i2, i3);
  score += DeterminePair(i3, i4);

  if (score == 9 || score == -9) {
    score *= 20;
  }

  return score * score * score;
}

inline int DeterminePair(int i1, int i2) {
  if (i1 == i2) {
    return i1 * 3;
  } else if (i1 != 0 && i2 == 0) {
    return i1;
  } else if (i1 == 0 && i2 != 0) {
    return i2;
  }
  return 0;
}
