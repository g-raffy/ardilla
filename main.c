#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

// #define DEBUG_MESSAGE(message, ...) (printf(message, __VA_ARGS__);)
#define DEBUG true
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


void error(char* pMessage)
{
    printf("ERROR: %s\n", pMessage);
    exit(1);
}

// array of pointers to structures
typedef struct
{
    int iNumElements;
    void **ppElements;
} Array;

void array_init(Array* pArray)
{
    pArray->iNumElements = 0;
    pArray->ppElements = NULL;
}

void array_empty(Array* pArray)
{
    if( pArray->iNumElements > 0 )
    {
        free(pArray->ppElements);
        pArray->ppElements = NULL;
        pArray->iNumElements = 0;
    }
}

/*
    adds an element to the array
*/
void array_add(Array* pArray, void* pElement)
{
    void** ppOldElements = pArray->ppElements;

    // allocate a bigger array of pointers
    pArray->ppElements = malloc(sizeof(void*) * (pArray->iNumElements + 1));

    // move the elements from the old elements to the new elements
    for(int iElement = 0; iElement< pArray->iNumElements; iElement++)
    {
        pArray->ppElements[iElement] = ppOldElements[iElement];
    }

    // now that its contants have been copied, free the old array of pointers
    if(ppOldElements != NULL)
    {
        free(ppOldElements);
        ppOldElements = NULL;
    }
    // free((void *)0x123456);

    // add a pointer to the new element at the end
    pArray->ppElements[pArray->iNumElements] = pElement;

    pArray->iNumElements++;

    
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

PieceDef * piece_def_create(PieceId iPieceId)
{
    PieceDef* pPiece = (PieceDef *)malloc(sizeof(PieceDef));
    pPiece->iPieceId = iPieceId;
    pPiece->iNumTiles = 0;
    pPiece->bHasHole = false;
    return pPiece;
}

void piece_def_delete(PieceDef* pPiece)
{
    debug_print("deleting piece %d\n", pPiece->iPieceId);
    free(pPiece);
}


void piece_def_print(PieceDef* pPiece)
{
    printf("piece %d : ", pPiece->iPieceId);
    for(int iTileIndex = 0; iTileIndex < pPiece->iNumTiles; iTileIndex++ )
    {
        printf(" (%d, %d)", pPiece->aTilesLocations[iTileIndex].iX, pPiece->aTilesLocations[iTileIndex].iY);
    }
    if (pPiece->bHasHole)
    {
        printf("(hole at %d, %d)", pPiece->sHolePos.iX, pPiece->sHolePos.iY);
    }
    printf("\n");
}

void piece_def_add_tile(PieceDef* pPiece, int iTilePosX, int iTilePosY)
{
    if( pPiece->iNumTiles >= MAX_PIECE_SIZE )
    {
        error("max number of tiles exceeded");
    }
    pPiece->aTilesLocations[pPiece->iNumTiles].iX = iTilePosX;
    pPiece->aTilesLocations[pPiece->iNumTiles].iY = iTilePosY;
    pPiece->iNumTiles++;
}

void piece_def_set_hole(PieceDef* pPiece, int iHolePosX, int iHolePosY)
{
    if( pPiece->bHasHole == true )
    {
        error("hole position has already been set");
    }
    pPiece->sHolePos.iX = iHolePosX;
    pPiece->sHolePos.iY = iHolePosY;
    pPiece->bHasHole = true;
}


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

void material_add_piece_def(Material* pMaterial, PieceDef* pPieceDef)
{
    // type de pMaterial: Material*
    // type de *pMaterial: Material
    // type de (*pMaterial).sPieceDefs: Array
    // type de pMaterial->sPieceDefs: Array
    // type de &(pMaterial->sPieceDefs): Array*
    // & should be read as 'address of'
    // * should be read as 'pointed by'
    // eg : 'Material* p' should be read as 'Material pointed by p'
    array_add(&(pMaterial->sPieceDefs), pPieceDef);
}

void material_delete(Material* pMaterial)
{
    // delete the piece definitions
    for( int iPieceIndex = 0 ; iPieceIndex < pMaterial->sPieceDefs.iNumElements; iPieceIndex++ )
    {
        PieceDef* pPiece = pMaterial->sPieceDefs.ppElements[iPieceIndex];
        piece_def_delete(pPiece);
        pMaterial->sPieceDefs.ppElements[iPieceIndex] = NULL;
    }
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

    // print the piece definitions
    for( int iPieceIndex = 0 ; iPieceIndex < pMaterial->sPieceDefs.iNumElements; iPieceIndex++ )
    {
        PieceDef* pPiece = pMaterial->sPieceDefs.ppElements[iPieceIndex];
        piece_def_print(pPiece);
    }
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

    PieceDef* p1 = piece_def_create(1);
    piece_def_add_tile(p1, 0, 0);
    piece_def_add_tile(p1, 1, 0);
    piece_def_add_tile(p1, 0, 1);
    piece_def_set_hole(p1, 0, 0);

    PieceDef* p2 = piece_def_create(2);
    piece_def_add_tile(p2, 0, 0);
    piece_def_add_tile(p2, 1, 0);
    piece_def_add_tile(p2, 0, 1);
    piece_def_set_hole(p2, 1, 0);

    PieceDef* p3 = piece_def_create(3);
    piece_def_add_tile(p3, 0, 0);
    piece_def_add_tile(p3, 1, 0);
    piece_def_set_hole(p3, 1, 0);

    PieceDef* p4 = piece_def_create(4);
    piece_def_add_tile(p4, 0, 0);
    piece_def_add_tile(p4, 1, 0);
    piece_def_set_hole(p4, 1, 0);

    PieceDef* p5 = piece_def_create(5);
    piece_def_add_tile(p5, 0, 0);

    material_add_piece_def(pMaterial, p1);
    material_add_piece_def(pMaterial, p2);
    material_add_piece_def(pMaterial, p3);
    material_add_piece_def(pMaterial, p4);
    material_add_piece_def(pMaterial, p5);

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