project('minesweeper-in-c', 'c')
src = ['minesweeper.c', 'board.c']
src_test = ['ncurses_demo.c', 'board.c']
dep = dependency('ncurses')
executable('minesweeper', src)
executable('ncurses_demo', src_test, dependencies : dep)
