# mintic
*Minimal Tic-Tac-Toe Solver*

![sample-text](https://img.shields.io/badge/build-sample%20text-brightgreen)
![works-on](https://img.shields.io/badge/works%20on-my%20mashine%E2%84%A2-orange)
![gotta-go-fast](https://img.shields.io/badge/gotta%20go-fast-blue)
![recursion](https://img.shields.io/badge/recursion-yes-red)

mintic is a minimalistic tic-tac-toe AI, which takes in a sequence of moves in a tic-tac-toe game and prints out the best possible following move. There are no flags and no interactive mode, you just give it the sequence as the only command line argument and it gives you the result and exits.

## How do I use it?

Every cell in the tic-tac-toe board is assigned a number, starting at 1, counting upwards from left to right and from top to bottom. The players alternate when making moves. Each move is denoted as the number of the cell, the player placed their symbol (usually either "X" or "O"). The sequence is the list of moves in order. The move "0" referes to a player skipping a turn.

```
 1 | 2 | 3      X | O |        X |   |
---+---+---    ---+---+---    ---+---+---
 4 | 5 | 6      O | X |          | X | O
---+---+---    ---+---+---    ---+---+---
 7 | 8 | 9      X |   | O        |   | X

 numbering      "127459"       "10569"
```

Given a sequence of moves, mintic returns the best following move. mintic can also return multiple results if they are seen as equally good.

```
$ mintic 127459
3
```

**Note:** As neither rotating nor flipping the board affects any game logic, you may also number the cells in a different manner. In case you mashine comes with a traditional num-pad, I highly recomment using that to punch in the sequence.

## Setup

Download and install:
```
git clone https://github.com/slerpyyy/mintic.git
cd mintic
make install
```

Run the program:
```
mintic <sequence>
```

## Why does this exist?
I got bored, don't judge me
