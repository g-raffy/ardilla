#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;


// array of pointers to structures
typedef struct
{
    int iNum_Elements;
    void *pElements;
} Array;

void array_init(Array* pArray)
{
    pArray->iNum_Elements = 0;
    pArray->pElements = NULL;
}

void array_empty(Array* pArray)
{
    if( pArray->iNum_Elements > 0 )
    {
        free(pArray->pElements);
        pArray->pElements = NULL;
        pArray->iNum_Elements = 0;
    }
}

typedef int PieceId;

typedef enum
{
    DIRECTION_UP    = 0,
    DIRECTION_LEFT  = 1,
    DIRECTION_DOWN  = 2,
    DIRECTION_RIGHT = 3,
} Direction;


typedef struct 
{
    int iX;
    int iY;
} Position;


/*
    description of the material used for a game (how many rows and columns in the borad, description of the pieces)
*/
typedef struct 
{
    // number of rows and columns of the board
    int iNumRows;
    int iNumCols;

    Array sPieceDefs; // array of PieceDef
    
    Array sHolesPos;  // array of Position
} Material;

Material * material_create(int iNumRows, int iNumCols)
{
    Material* pMaterial = (Material *)malloc(sizeof(Material));
    pMaterial->iNumRows = iNumRows;
    pMaterial->iNumCols = iNumCols;
    array_init(&pMaterial->sPieceDefs);
    array_init(&pMaterial->sHolesPos);
    return pMaterial;
}

void material_delete(Material* pMaterial)
{
    // type of pMaterial : Material*
    // type of *pMaterial : Material
    // type of (*pMaterial).sPieceDefs : Array
    // type of pMaterial->sPieceDefs : Array
    // type of &(pMaterial->sPieceDefs) : Array*
    array_empty(&(pMaterial->sPieceDefs));
    array_empty(&(pMaterial->sHolesPos));
    free(pMaterial);
}

void material_print(Material* pMaterial)
{
    printf("board size : %d x %d\n", pMaterial->iNumRows, pMaterial->iNumCols);
}


typedef struct
{
    bool bCellIsCovered;
    bool bCellHasNut;
} CellState;


// state of every cell in the board
typedef struct 
{
    Material* pMaterial; // reference to the material used

    CellState* pCellState;
} Board;

typedef struct
{
    PieceId iPieceId; // the piece being moved
    Direction eMoveDir;  // the direction in which the piece is moved
    bool bNutHasFallen;
} Move;

// maximum number of tiles covered by any piece
#define MAX_PIECE_SIZE 3

typedef struct 
{
    PieceId iPieceId;
    int iNumTiles;
    Position aTilesLocations[MAX_PIECE_SIZE];
    bool bHasHole;
    Position sHolePos;
} PieceDef;

typedef enum
{
    ROTATION_0   = 0,
    ROTATION_90  = 1,
    ROTATION_180 = 2,
    ROTATION_270 = 3
} Rotation;

typedef struct 
{
    Position sPos;
    Rotation eRot;
} PiecePos;

typedef struct
{

} Piece;


// stack of moves
typedef struct 
{
    Move *pMoves;
    int iNumMoves;
} Game;


/*
    a challenge made of initial position of pieces
*/
typedef struct
{
    Material* pMaterial;
    Array sPieceStartConfig;  // array of PiecePos
} Puzzle;


void board_play_move(Board* pBoard, Move* pMove)
{
    // don't forget to set pMove->bNutHasFallen
}

void board_unplay_move(Board* pBoard, Move* pMove)
{
    // don't forget to use pMove->bNutHasFallen
}

void game_push_move(Game* pGame, Move sMove, Board* pBoard)
{
    board_play_move(pBoard, &sMove);

    pGame->pMoves[pGame->iNumMoves] = sMove;
    pGame->iNumMoves++;
}

void game_pop_move(Game* pGame, Board* pBoard)
{
    pGame->iNumMoves--;
    Move* pLastMove = &(pGame->pMoves[pGame->iNumMoves]);

    board_unplay_move(pBoard, pLastMove);
}

#ifdef ENABLE_UNFINISHED_CODE

void scan(Game* pGame, Board* pBoard, Game** ppSolutions, int* piNumSolutions)
{
    if( game_is_solution() == true )
    {
        // copy this game to solutions array
        Game *pSolution = game_duplicate(pGame);
        ppSolutions[*piNumSolutions] = pSolution;
        (*piNumSolutions)++;
    }
    else
    {
        // explore deeper
        int iPieceId;
        int iDir;

        for(iPieceId = 0; iPieceId < 4; iPieceId++)
        {
            for(iDir = 0; iDir < 4; iDir++)
            {
                Move sMove;
                sMove.iPieceId = iPieceId;
                sMove.eMoveDir = iDir;
                if( move_is_valid(sMove, pGame, pBoard) )
                {
                    game_push_move(pGame, sMove, pBoard);
                    scan(pGame, pBoard, ppSolutions, piNumSolutions);
                    game_pop_move(pGame, pBoard);
                }
            }
        }
    }
}

void find_solutions(Board* pBoard, PieceDef* pPieceDefs, PiecePos* pPiecesPos, int iNumPieces, Game** ppSolutions, int* piNumSolutions)
{
    Game* pWorkGame = game_create();
    scan(pWorkGame, pBoard, ppSolutions, piNumSolutions);
    game_delete(pWorkGame);
}

#endif

int main(int argc, char* argv[])
{
    Material* pMaterial = material_create(4, 4);
    material_print(pMaterial);
    material_delete(pMaterial);
    return 0;
}

#ifdef TOTO
void toto(void)
{
    Material a;
    Material* b;
    Material** c;
    a.iNumCols = 5;
    // type of b   : Material*
    // type of *b  : Material
    // type of &b  : Material**
    // type of &&b : Material***

    // type of &&c : Material****
    // type of &c  : Material***
    // type of c   : Material**
    // type of *c  : Material*
    // type of **c : Material

    // type of *&c : Material**
    // type of &*c : Material**
}
#endif // TOTO