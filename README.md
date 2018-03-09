# sudokusolver

Run the "play_sudoku.c" file.
Add your Sudoku problem in the form as given below:
--2-6-7--
--32-8--5
581--7-63
7-----6--
34-6-9-12
--6-----4
13-5--496
8--1-45--
--4-9-1--

Use "-" for the empty spaces.
After you input the sudoku, you have multiple options:
1. fill: type "fill row-number col-number number" which will try to fill in the position (row,col) the number if it's possible without any constraints.
2. choices: type "choices row-number col-number" which will return all the possible values that can be inserted into the given position(row,col)
3. erase: type "erase row-number col-number" to erase the number in the given position(row,col). 
4. solve: type "solve" to solve the sudoku and print the final solution.

Note: Row and Coloumn starts with 0.
