# ConnectFourMiniMax
MiniMax AI written in C for the game Connect Four

# How To Compile & Run The Code #
In a terminal with G++ installed (e.g. Max/Linux), input the following commands:

#To Compile#
make clean
make

#To Run#
./main

### Evaluation Logic ###
The evaluation function can be found at the top of ai.c The outdated OpenCL version of the evaluation function can be found in evalutor.cl, however a strict C approach was taken in the end
as it performed better overall.

To evaluate, the logic goes through every possible 'four in a row' win condition. It takes advantage of ConnectFour being a zero-sum game in order to handle it's evaluating technique.

For every possible row that someone can win with, the evaluation method scores each row then sums up the scores.
All the X's in the row are counted, then all the O's. The general algorithm is:
score = (XCount * XCount * XCount) - (YCount * YCount * YCount)

If the numbers are cubed (1, 8, 27, 64), we know that there are either all X's and blanks in the line, or O's and blanks.
If the numbers are not cubed, we know there was a disconnect (there was both a X and a Y in the line), and the score is dropped to zero.
If the number is 64 or -64 (Win for X or O), we multiple the score by 100,000.

Example scores are below:
XXXX = (4*4*4) - 0 == 64 points       WinForX, 64 * 100,000 = 6,400,000 points
XX-X = (3*3*3) - 0 = 27 points        27 is cubed. +27 points
XX-- = (2*2*2) - 0 = 8 points         8 is cubed. +8 points
OO-- = 0 - (2*2*2) = -8 points        -8 is cubed. -8 points
OOOO = 0 - (4*4*4) == -64 points      WinForO, -64 * 100,000 = -6,400,000 points
XXO- = (2*2*2) - (1*1*1) = 7 points   7 is not cubed. 0 points

Following this scoring, boards are evaluated equally. No favortism is shown to either team. The further in the negatives a score is,
the more it favours team O (2nd player), while the higher the score is, the more if favours team X (1st player)

### Minimax Logic ###
The Min and Max functions can be found in ai.c.

The minimax algorithm used is near straight minimax. As we go down, the system evaluates a board at each step of the way.
If it finds a board results in either player winning or losing, it scores that board immediately rather than continuing to go deeper
in order to prune the tree a bit.

### Formal Pruning ###
The official pruning implemented in this ConnectFour AI is one of early-evaluations.

From Depth 1-3, all nodes are branched on indiscriminately, regardless of score
On depth 4, the worst-predicted option of the 7 possible modes is trimmed
For depths 5+, the predicted scores of the nodes are averaged, with the "average of better" predicted nodes being the only ones it branches further on

### Depth ###
Currently, the system is set to search a depth of 11. This can be changed in main.c around lines 32/35.

The system starts off slow, as it can't prune the early branches well, but speeds up exponentially as the game goes on.
