#ifndef ai_h
#define ai_h

void initializeAI(int val, int maxDepth);

//BoardData for 1 board
int* pickMove(int* boardData);

//BoardData for 1 board
int* determineSubBoards(int* boardData, int* boardCount, int piece);

int* createBoardIfPossible(int* boardData, int x, int* counter, int piece);

void boardCopy(int* source, int* destination, int sourceBoardOffset, int destinationBoardOffset);

//returns highest evaluated board state
int* max(int* boardData, int* score, int depth);

//returns lowest evaluated board state
int* min(int* boardData, int* score, int depth);


#endif /* ai_h */
