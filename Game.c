

#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include <stdlib.h>




Game* getGameInitParams(Game* game, int w, int h){
    if(*game==NULL) dealloc(game);
    unsigned int size=w*h*w*h;
    int hints;
    char c;
    Cell *gameBoard;
    int *solution;
    printf("Please enter the number of cells to fill [0-%d]:\n",size-1);
    while(scanf("%d%c",&hints,&c)!=2){
        printf("Error: Invalid number of cells to fill (should be between 0 and %d)\n",size);
        printf("Please enter the number of cells to fill [0-%d]:\n",size-1);
    }
    if((gameBoard =(Cell*)calloc(size,sizeof(Cell)))==NULL){
        printf("Error: getGameInitParams has failed\n");
        return NULL;
    }
    if((solution =(int*)calloc(size,sizeof(int)))==NULL){
        printf("Error: getGameInitParams has failed\n");
        return NULL;
    }
    game->blockHeight=h;
    game->blockWidth=w;
    game->solution=solution;
    game->board=gameBoard;
    randomSolve(&game);
    addHints(&game);

    return &game;
}


int checkBlock(Game game,int x, int y, int value){
    int k,r;
    int boardIndex = getBoardIndex(game,x,y);
    int blockStart;
    int s=blockStart;
    while(x%game.blockWidth!=0)x--;
    while(y%game.blockHeight!=0)y--;
    blockStart = getBoardIndex(game,x,y);
    for(k=0;k<game.blockHeight;k++){
        for(r=0;r<game.blockWidth;r++){
            if(game.board[s].value==value && s!=boardIndex)
                return 0;
            blockStart++;
        }
        s=blockStart+game.blockWidth*game.blockHeight-1;
    }
    return 1;

}

int getBoardIndex(Game game, int x, int y){
    return game.blockHeight*game.blockWidth*(y-1)+(x-1);
}

int checkLegal(Game game,int x, int y, int value){
    return checkBlock(game,x,y,value)*checkRowColumn(game,x,y,value);
}

int checkRowColumn(Game game, int x, int y, int value) {
    int index = getBoardIndex(game, x, y);
    int  line = getBoardIndex(game, 1, y);
    int row = getBoardIndex(game, x, 1);
    for (line; line < line + game.blockHeight*game.blockWidth; line++) {
        if (game.board[line].value == value) {
            if (line != index) {
                return 0;
            }
        }
    }
    for (row; row < game.boardSize; row+=(game.blockHeight*game.blockWidth)) {
        if (game.board[row].value == value) {
            if (row != index) {
                return 0;

            }
        }
    }
    return 1;
}

/*a[0]=x, a[1]=y*/
int* position(Game game ,int index) {
    int a[2];
    a[1] = index % (game.blockHeight*game.blockWidth);
    a[0] = (index - a[1]) / (game.blockHeight*game.blockWidth);


    return a;
}

int findFirstNotFixed(Game game) {
    int i = 0;
    for ( ; i < game.boardSize; i++) {
        if (game.board[i].isFixed != 1 && game.board[i].isPlayerMove != 1) {
            return i;
        }
    }
    return i;
}

int*getAllPossibleValues(Game* game,int*pastValues, int x,int y){
    int *values = NULL;
    int i,k=1;
    for(i=1;i<=game->blockWidth*game->blockHeight;i++) {
        if (checkLegal(game, x, y, i) && !inArray(pastValues)) {
            values = (int *) realloc(values, k * sizeof(int));
            if (values == NULL)return {0};
            values[k - 1] = i;
            k++;
        }
    }
    return values;
}


int set(Game game,int x, int y, int  value) {

	int index = getBoardIndex(game, x, y);
	if (game.board[index].isFixed) {
		printf("Error: cell is fixed\n");
		return;
	}
	if (value == 0)
		game.board[index].value = 0;
	if (game.solution[index] == value) {
		game.board[index].value = value;
		game.board[index].isPlayerMove = 1;
	}
	else if (checkBlock(game, x, y, value) && checkRowColumn(game, x, y, value)) {
		game.board[index].value = value;
	}
	else {
		printf("Error: value is invalid\n");
	}
	return winningBoard(game);


}

int validate(Game game) {
	if (detSolve(game)) {
		printf("Validation passed: board is solvable\n");
	}
	else {
		printf("Validation failed: board is unsolvable\n");
	}

}
int winningBoard(Game game) {
	for (int i = 0;i< game.boardSize; i++) {
		if (game.board[i].value) {
			return 0;
		}
	}
	return 1;
}

int exitGame(Game* game){
    dealloc(game);
    free(game);
    return -1;
}



