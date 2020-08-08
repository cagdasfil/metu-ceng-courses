/* Explanations *************************************************************************************************************************************
|
| The Tic-Tac-Toe board has 9 cells. We will assume that the cell numbering is as follows:
|
|   c0 | c1 | c2
|   ------------
|   c3 | c4 | c5
|   ------------
|   c6 | c7 | c8
|
****************************************************************************************************************************************************/




/* Includes ****************************************************************************************************************************************/

#include "Utilities.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>   /* For standard input/output functions */

/***************************************************************************************************************************************************/




/* Definitions *************************************************************************************************************************************/

#define AUTHOR  ( "Çağdaş Fil" )  /* Author's name                                   */

#define EMPTY   ( ' '             )  /* Label for empty cells                           */
#define PLAYER1 ( 'X'             )  /* Label for player 1 and cells marked by player 1 */
#define PLAYER2 ( 'O'             )  /* Label for player 2 and cells marked by player 2 */

/***************************************************************************************************************************************************/




/* Type Definitions ********************************************************************************************************************************/

typedef unsigned char boolean ;

/***************************************************************************************************************************************************/




/* Global Variables ********************************************************************************************************************************/

char c0 , c1 , c2 , c3 , c4 , c5 , c6 , c7 , c8 ;  /* Cells of the Tic-Tac-Toe board */

/***************************************************************************************************************************************************/




/* Functions ***************************************************************************************************************************************/

/* Initializes the global variables. */

void initializeGlobalVariables ( void )
{
  /* TODO */
  /* ...  */
    c0 = c1 = c2 = c3 = c4 = c5 = c6 = c7 = c8 = ' ';

}

/*=================================================================================================================================================*/

/* Prints a banner including the author name. */

void printBanner ( void )
{
  printf(                                                                                   "\n"          ) ;
  printf( "*******************************************************************************" "\n"          ) ;
  printf( "* Tic-Tac-Toe                                                                 *" "\n"          ) ;
  printf( "* by %-75s *"                                                                    "\n" , AUTHOR ) ;
  printf( "*******************************************************************************" "\n"          ) ;
}

/*=================================================================================================================================================*/

/* Prints the current board configuration. */

void printBoard ( void )
{
  /* TODO */
  /* ...  */
    printf("\n    Cells                  Board\n\
.-----------.          .-----------.\n\
| 0 | 1 | 2 |          | %c | %c | %c |\n\
|-----------|          |-----------|\n\
| 3 | 4 | 5 |          | %c | %c | %c |\n\
|-----------|          |-----------|\n\
| 6 | 7 | 8 |          | %c | %c | %c |\n\
'-----------'          '-----------'\n\n",c0,c1,c2,c3,c4,c5,c6,c7,c8);

}

/*=================================================================================================================================================*/

/* Gets the next move of "player", and marks it on the board.                     */
/* Loops (and asks again) if the input is not valid, or if the cell is not empty. */

void getNextMove ( char player )
{
    int position,i,validity;
  /* TODO */
  /* ...  */
  do{

    do{
	     printf("Player %c, enter your next move (0-8) : ",player);
	     validity = scanf("%d",&position);
	     if (validity == 0){
	        printf("\nIncorrect input! Please try again.\n\n");
	        clearInputBuffer();
	        continue;
	     }

	     if(position<9 && position>-1){
         break;
       }

	  printf("\nIncorrect input! Please try again.\n\n");
    }
    while (1);

	if( position == 0 && c0 == ' '){
	    c0=player;
	    return ;
	}

	else if( position == 1 && c1 == ' '){
	    c1=player;
	    return ;
	}


	else if( position == 2 && c2 == ' '){
	    c2=player;
	    return ;
	}

	else if( position == 3 && c3 == ' '){
	    c3=player;
	    return ;
	}


	else if( position == 4 && c4 == ' '){
	    c4=player;
	    return ;
	}

	else if( position == 5 && c5 == ' '){
	    c5=player;
	    return ;
	}


	else if( position == 6 && c6 == ' '){
	    c6=player;
	    return ;
	}


	else if( position == 7 && c7 == ' '){
	    c7=player;
	    return ;
	}


	else if( position == 8 && c8 == ' '){
	    c8=player;
	    return ;
	}
	else{
	    printf("\nCell is not empty! Please try again.\n\n");
	}
    }
    while(1);
}

/*=================================================================================================================================================*/

/* Checks if one of the players wins the game, or if the board is full and the game ended with a draw. Prints how the game ended. */
/* Returns TRUE if the game ended, otherwise, does not print anything and just returns FALSE.                                     */

boolean checkAndPrintGameResult ( void )
{
  /* TODO */
  /* ...  */
    if(c0==c1 && c0==c2 && c1==c2 && c0!=' '){
	printf("Player %c WINS (top row)!\n",c0);
	return TRUE;
    }
    if(c3==c4 && c3==c5 && c4==c5 && c3!=' '){
	printf("Player %c WINS (middle row)!\n",c3);
	return TRUE;
    }
    if(c6==c7 && c6==c8 && c7==c8 && c6!=' '){
	printf("Player %c WINS (bottom row)!\n",c6);
	return TRUE;
    }
    if(c0==c3 && c0==c6 && c3==c6 && c0!=' '){
	printf("Player %c WINS (left column)!\n",c0);
	return TRUE;
    }
    if(c1==c4 && c1==c7 && c4==c7 && c1!=' '){
	printf("Player %c WINS (middle column)!\n",c1);
	return TRUE;
    }
    if(c2==c5 && c2==c8 && c5==c8 && c2!=' '){
	printf("Player %c WINS (right column)!\n",c2);
	return TRUE;
    }
    if(c0==c4 && c0==c8 && c4==c8 && c0!=' '){
	printf("Player %c WINS (primary diagonal)!\n",c0);
	return TRUE;
    }
    if(c2==c4 && c2==c6 && c4==c6 && c2!=' '){
	printf("Player %c WINS (secondary diagonal)!\n",c2);
	return TRUE;
    }
    if(c0!=' ' && c1!=' ' && c2!=' ' && c3!=' ' && c4!=' ' && c5!=' ' && c6!=' ' && c7!=' ' && c8!=' '){
	printf("DRAW!\n");
	return TRUE;
    }
    return FALSE;
}

/***************************************************************************************************************************************************/




/* Main Function ***********************************************************************************************************************************/

/* Initializes the global variables.                                                                                                          */
/* Prints banner.                                                                                                                             */
/* Randomly chooses one of the players to start first.                                                                                        */
/* Asks each player to make a move in turn until the game finishes (prints the board configuration each time before a player makes her move). */
/* Prints the game result.                                                                                                                    */
/* Prints a goodbye message.                                                                                                                  */
/* Prints the number of random numbers that were generated during the game play.                                                              */

int main ( void )
{
  char currentPlayer = ( ( randomIntegerMinMaxUnBiased( 0 , 1 ) == 0 ) ? PLAYER1 : PLAYER2 ) ;

  initializeGlobalVariables() ;
  printBanner              () ;
  printBoard               () ;

  do
  {
    getNextMove( currentPlayer ) ;
    printBoard (               ) ;

    if   ( currentPlayer == PLAYER1 )  { currentPlayer = PLAYER2 ; }
    else                               { currentPlayer = PLAYER1 ; }
  }
  while ( checkAndPrintGameResult() == FALSE ) ;

  printf( "\nGood game. Have a nice day!\n" ) ;

  printf( "\n%d random numbers were generated during the game play.\n\n" , numberOfRandomNumbersGenerated ) ;

  return 0 ;
}

/***************************************************************************************************************************************************/
