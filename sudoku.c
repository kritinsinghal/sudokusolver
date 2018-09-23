#include "solver.h"
#include "sudoku.h"
#include "array.h"

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};

// unique(v) checks if all the values in the array v is unique or not
// effect: returns bool
bool unique(const int v[]) {
  int b[DIM+1];
  for(int i=0;i<DIM;i++){
    b[v[i]]++;
  }
  for(int i=1;i<=DIM;i++) {
    if(b[i]>1) {
      return false;
    }
  }
  return true;
}

// get_row(s,r,row,col) modifies r with all the elements in s of
//      the row.
// effect: modifies r
void get_row(const struct sudoku *s, int r[], const int row, const int col) {
  for(int i=0;i<DIM;i++) {
    r[i]=s->sol[row*9+i];
  }
}

// get_row(s,r,row,col) modifies r with all the elements in s of
//      the col.
// effect: modifies r
void get_col(const struct sudoku *s, int r[], const int row, const int col) {
  for(int i=0;i<DIM;i++) {
    int p=i*DIM+(col%DIM);
    r[i]=s->sol[p];
  }
}

// get_row(s,r,row,col) modifies r with all the elements in s of
//      the box.
// effect: modifies r
void get_box(const struct sudoku *s, int r[], int row, int col) {
    int i;
    for(i=0;i<3;i++){
      if((row<3)&&(col<3)) {
        r[i*3]=s->sol[0+(i*DIM)];
        r[(i*3)+1]=s->sol[1+(i*DIM)];
        r[(i*3)+2]=s->sol[2+(i*DIM)];
      } else if ((row<3)&&(col<6)) {
        r[i*3]=s->sol[3+(i*DIM)];
        r[(i*3)+1]=s->sol[4+(i*DIM)];
        r[(i*3)+2]=s->sol[5+(i*DIM)];
      } else if ((row<3)&&(col<9)) {
        r[i*3]=s->sol[6+(i*DIM)];
        r[(i*3)+1]=s->sol[7+(i*DIM)];
        r[(i*3)+2]=s->sol[8+(i*DIM)];
      } else if ((row<6)&&(col<3)) {
        r[i*3]=s->sol[0+((i+3)*DIM)];
        r[(i*3)+1]=s->sol[1+((i+3)*DIM)];
        r[(i*3)+2]=s->sol[2+((i+3)*DIM)];
      } else if ((row<6)&&(col<6)) {
        r[i*3]=s->sol[3+((i+3)*DIM)];
        r[(i*3)+1]=s->sol[4+((i+3)*DIM)];
        r[(i*3)+2]=s->sol[5+((i+3)*DIM)];
      } else if ((row<6)&&(col<9)) {
        r[i*3]=s->sol[6+((i+3)*DIM)];
        r[(i*3)+1]=s->sol[7+((i+3)*DIM)];
        r[(i*3)+2]=s->sol[8+((i+3)*DIM)];
      } else if ((row<9)&&(col<3)) {
        r[i*3]=s->sol[0+((i+6)*DIM)];
        r[(i*3)+1]=s->sol[1+((i+6)*DIM)];
        r[(i*3)+2]=s->sol[2+((i+6)*DIM)];
      } else if ((row<9)&&(col<6)) {
        r[i*3]=s->sol[3+((i+6)*DIM)];
        r[(i*3)+1]=s->sol[4+((i+6)*DIM)];
        r[(i*3)+2]=s->sol[5+((i+6)*DIM)];
      } else if ((row<9)&&(col<9)) {
        r[i*3]=s->sol[6+((i+6)*DIM)];
        r[(i*3)+1]=s->sol[7+((i+6)*DIM)];
        r[(i*3)+2]=s->sol[8+((i+6)*DIM)];
      }
    }
}    

// fill_cell(s,row,col,num) tries to fill num in the cell (row,col).  
// It returns 0 if doing so does not violate any of the row, column, 
// and box constraints.  Otherwise, it returns a negative integer.
// requires: row and col are valid indices. 
//           num is an integer between 1 and 9.
// effects:  mutates *s
int fill_cell(struct sudoku *s, int row, int col, int num) {
  int box[DIM],r[DIM],c[DIM];
  s->sol[row*DIM+col]=num;
  get_row(s,r,row,col);
  get_col(s,c,row,col);
  get_box(s,box,row,col);
  if((unique(r)==true)&&(unique(c)==true)&&(unique(box)==true)) {
    return 0;
  }
  return ERROR;  
}


  
// choices_cell(s,row,col,choices,num_choices) determines all the 
// possible values for the cell (row,col) that do not violate
// any of the row, column, and box constraints.  It mutates choices 
// to contain the possible values and mutates *num_choices to be
// the number of possible values.
// requires: row and col are valid indices.
//           choices is a valid array with length DIM
// effects:  mutates choices and *num_choices
void choices_cell(const struct sudoku *s, int row, int col, 
                 int choices[], int *num_choices) {
  int x=0;
  struct sudoku p;
  for(int i=0;i<DIM*DIM;i++) {
    p.sol[i]=s->sol[i];
  }
  for(int i=MIN;i<=MAX;i++) {
    if(fill_cell(&p,row,col,i)==0){
      choices[x++]=i;
    }
  }
  *num_choices=x;
}

// solved_puzzle(s) returns true if s has a valid solution to the puzzle, 
// and returns false otherwise.
bool solved_puzzle(const struct sudoku *s) {
  // Implement this function
  int box[9],r[9],c[9];
  for(int i=0;i<=80;i++) {
    if(s->sol[i]==0) {
      return false;
    }
    get_row(s,r,i%DIM,i);
    get_col(s,c,i%DIM,i);
    get_box(s,box,i%9,i);
    if((unique(r)==false)||(unique(c)==false)||(unique(box)==false)) {
      return false;
    }
  }
  return true;
}

// find_empty(sol,&row,&col) finds the first empty position in sol
// effect: mutates row and col
bool find_empty(int sol[],int *row, int *col) {
  for(int i=0;i<DIM*DIM;i++) {
    if(sol[i]==0) {
      *row=i%9;
      *col=i/9;
      return true;
    }
  }
  return false;
}

// solve(s) solves the Sudoku puzzle by search and backtracking.  
// It mutates the 
// It returns true if s has a valid solution to the puzzle, 
// and returns false otherwise.
// effects: mutates *s
bool solve(struct sudoku *s) {
  // Implement this function
  int row=0,col=0,ans;
  if(solved_puzzle(s)) {
    return true;
  }
  int pos=0;
  while(pos<DIM*DIM && s->sol[pos]!=0) {
    ++pos;
  }
  if(pos==DIM*DIM) {
    return false;
  }
  row=pos/DIM;
  col = pos%DIM;
  int choices[DIM];
  int num_choices;
  choices_cell(s,row,col,choices,&num_choices);
  if(num_choices==0) {
    return false;
  }
  for(int i=0;i<num_choices;i++) {
    ans=fill_cell(s,row,col,choices[i]);
    if(solve(s)==true) {
      return true;
    }
  }
  s->sol[pos]=0;
  return false;
}
  

// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
        scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}
