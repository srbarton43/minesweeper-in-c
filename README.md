# Minesweeper in C

Sam Barton, April 2023

## What is this?

A console version of minesweeper, one of my favorite games to play in my spare time.

## How to Play?

There are two gamemodes: automatic and manual...

In manual mode, an action must be specified as either 'click' using 'c' or 'flag' using 'f'. Then a tile must be specified using its <char><num> mapping.

In automatic mode, the action is determined by the square clicked. If the tile is still covered or fflagged, the command will toggle that tile's flag. If the clicked tile is uncovered, the logic is slightly more complicated; if the tile is touching more flags than its displayed number, then all adjacent tiles are cleared, if not, nothing happens.

## Game Over

![game over screenshot](img/game.png)

## Project Structure

```markdown
.
├── .gitignore
├── Makefile
├── README.md
├── img
│   └── game.png
├── minesweeper
│   ├── .gitignore
│   ├── Makefile
│   ├── board.c
│   ├── board.h
│   └── minesweeper.c
└── tests
    ├── .gitignore
    └── testing.sh

4 directories, 11 files
```

