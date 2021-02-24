//OOSD Lab Assignmnet
//Name - Bidyut Mondal
//Roll No - 18MA20010 

#include<iostream>
#include<stdlib.h>

using namespace std;


//Move is used to denote the position of a cell in the board
struct Move{ 
    int row, col; 
}; 
 

char computer, player; //computer and player are defined globally to assign "X" to whoever goes first

//position array is used to map the position given by the player to appropriate cell of the board
int position[9][2]={{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};


//This drawBoard function is used to print the board every time after the user plays a move 
void drawBoard(char board[3][3]){
    cout<<"----++---++----"<<endl;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<"| ";
            if(board[i][j] == '-'){
                cout<<" "<<" |";
            }
            else{
                if(board[i][j]=='X') cout<<"X |";
                else cout<<"O |";
            }
        }
        cout<<"\n----++---++----"<<endl;
    }
}

  
/*
This iBoardEmpty function is used to check if the board is empty or not.
It returns true if the any cell is empty.
Returns false if the the board is full. 
*/ 
bool isBoardEmpty(char board[3][3]){
    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            if (board[i][j]=='-') return true;
        }
    }
    return false; 
} 
 

/*
This winner function is returns 1 if the computer wins, -1 if the player wins and 0 if nobody wins.
We are return 1 and -1 becausing this function is further used in optimisation where we need to subtract or add depth.
Since the max depth of the board can be 9. So 1 seems to be fair choice.
*/
int winner(char board[3][3]){ 
    for (int i = 0; i<3; i++){ //checks if anyone wins or not in each column 
        if (board[i][0]==board[i][1] && board[i][1]==board[i][2]){ 
            if (board[i][0]==computer) return 1; 
            else if (board[i][0]==player) return -1; 
        } 
    } 
  
    for (int j = 0; j<3; j++){ //checks if anyone wins or not in each row 
        if (board[0][j]==board[1][j] && board[1][j]==board[2][j]){ 
            if (board[0][j]==computer) return 1; 
            else if (board[0][j]==player) return -1; 
        } 
    } 

    if (board[0][0]==board[1][1] && board[1][1]==board[2][2]){ //diagonal checking 
        if (board[0][0]==computer) return 1; 
        else if (board[0][0]==player) return -1; 
    } 
    if (board[0][2]==board[1][1] && board[1][1]==board[2][0]){ //diagonal checking 
        if (board[0][2]==computer) return 1; 
        else if (board[0][2]==player) return -1; 
    } 
    return 0; 
}  


//This opposite function is used to find the opposite cell of a cell.
Move opposite(int p){
    Move pos;
    if(p==1){
        pos.row = 2; pos.col = 2;
        return pos;
    }
    else if(p==3){
        pos.row = 2; pos.col = 0;
        return pos;
    }
    else if(p==7){
        pos.row = 0; pos.col = 2;
        return pos;
    }
    else if(p==9){
        pos.row = 0; pos.col = 0;
        return pos;
    }
    return pos;
}


//This function is called to take valid moves only
//Cells already filled cannot be entered again
void allowedMove(char board[3][3], int mv){
    while(board[position[mv-1][0]][position[mv-1][1]] != '-'){
        cout<<"Space filled. (Choose only form the blank spaces) : \n";
        cin>>mv;
    }
    return;
} 


//This function is used to check whether the computer can win by giving a specific move.
//It returns the position of cell where the computer can give input and win.
Move attack(char board[3][3]){
    Move pos;
    for(int i=0; i<3 ; i++){
        for(int j=0; j<3 ; j++){
            if(board[i][j]=='-'){
                board[i][j]=computer;
                if(winner(board)==1){
                    pos.row = i;
                    pos.col = j;
                    return pos;
                }
                board[i][j]='-';
            }
        }
    }
    pos.row = -1;
    pos.col = -1;
    return pos;
}


//This function is used to check whether the player can win by giving a specific move.
//So the computer blocks that spot and prevent the player from winning.
//It returns the position of cell where the computer can give input and stop the player from winning.
Move defend(char board[3][3]){
    Move pos;
    for(int i=0; i<3 ; i++){
        for(int j=0; j<3 ; j++){
            if(board[i][j]=='-'){
                board[i][j]=player;
                if(winner(board)==-1){
                    pos.row = i;
                    pos.col = j;
                    return pos;
                }
                board[i][j]='-';
            }
        }
    }
    pos.row = -1;
    pos.col = -1;
    return pos;
}


//This function returns a position from the where by which the game can continue.
//It also prevents the player playing from winning.
Move stay(char board[3][3]){
    Move pos;
    for(int i=0; i<3 ; i++){
        for(int j=0; j<3 ; j++){
            if(board[i][j]=='-'){
                pos.row = i;
                pos.col = j;
                return pos;
            }
        }
    }
    return pos;
}


//This function is used when the player/user gives input first.
void automateBoard1(char board[3][3]){
    int mv;
    Move pos;
    cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
    cin>>mv;
    board[position[mv-1][0]][position[mv-1][1]] = player; 
    
    if(mv==5){              //Cases when the player gives the centre position as the 1st input
        board[0][0] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv==9){
            board[0][2] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
        while(true){
            pos=attack(board);
            if(pos.row != -1){
                board[pos.row][pos.col] = computer;
                drawBoard(board);
                cout<<"YOU LOST :( .\n";
                return;
            }
            pos = defend(board);
            if(pos.row == -1) pos = stay(board);
            board[pos.row][pos.col] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            if(!isBoardEmpty(board)) break;
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;                
        }            
        cout<<"IT IS A DRAW :)\n";
        return;
    }
    else if(mv==1 or mv==3 or mv==7 or mv==9){          //Cases when the player gives any corner postion as 1st input
        Move opp = opposite(mv);
        board[1][1] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(position[mv-1][0]==opp.row and position[mv-1][1]==opp.col){
            board[0][1] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;
                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
        else{
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;                
        }
    }
    else if(mv==2 or mv==4){            //Cases when the user gives a 2nd or 4th position as 1st input 
        board[0][0] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv != 5){
            board[1][1] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
        else{
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
    }

    else{               //rest all cases
        board[2][2] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv != 5){
            board[1][1] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( .\n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
        else{
            while(true){
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( \n";
                    return;
                }
                pos = defend(board);
                if(pos.row == -1) pos = stay(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;                
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }
    }  
}


//This function is used when the computer gives the first input.
void automateBoard2(char board[3][3]){
    int mv;
    Move pos;
    cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
    cin>>mv;
    allowedMove(board, mv);
    board[position[mv-1][0]][position[mv-1][1]] = player;
    if(mv==5){              //Cases when the player gives the centre position as the 1st input
        board[2][2] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv==3 or mv==7){
            Move opp = opposite(mv);
            board[opp.row][opp.col] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            pos = attack(board);
            board[pos.row][pos.col] = computer;
            drawBoard(board);
            cout<<"YOU LOST :( \n";
            return;
        }
        else{
            while(true){
                pos = defend(board);
                board[pos.row][pos.col] = computer;
                cout<<"\nBoard after computer's move:\n";
                drawBoard(board);
                if(!isBoardEmpty(board)) break;
                cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
                cin>>mv;
                allowedMove(board, mv);
                board[position[mv-1][0]][position[mv-1][1]] = player;
                pos=attack(board);
                if(pos.row != -1){
                    board[pos.row][pos.col] = computer;
                    drawBoard(board);
                    cout<<"YOU LOST :( \n";
                    return;
                }
            }            
            cout<<"IT IS A DRAW :)\n";
            return;
        }

    }

    else if(mv==4 or mv==7){            //Cases when the player gives the 4th or 7th position as the 1st input
        board[0][2] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv==2){
            board[2][2] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            pos = attack(board);
            board[pos.row][pos.col] = computer;
            drawBoard(board);
            cout<<"YOU LOST :( \n";
            return;
        }
        else{
            board[0][1] = computer;
            drawBoard(board);
            cout<<"YOU LOST :( \n";
            return;
        }

    }

    else{               //rest all cases
        board[2][0] = computer;
        cout<<"\nBoard after computer's move:\n";
        drawBoard(board);
        cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
        cin>>mv;
        allowedMove(board, mv);
        board[position[mv-1][0]][position[mv-1][1]] = player;
        if(mv==4){
            board[2][2] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            cout<<"Enter Your Move (Choose only form the blank spaces) : \n";
            cin>>mv;
            allowedMove(board, mv);
            board[position[mv-1][0]][position[mv-1][1]] = player;
            pos = attack(board);
            board[pos.row][pos.col] = computer;
            drawBoard(board);
            cout<<"YOU LOST :( \n";
            return;
        }
        else{
            board[1][0] = computer;
            drawBoard(board);
            cout<<"YOU LOST :( \n";
            return;
        }
    }        
}


int main(){
    char play = 'y';
    while(play=='y'){
        cout <<"\nWelcome to TicTacToe:\n";
        char board[3][3]={{'-','-','-'},{'-','-','-'},{'-','-','-'}};           //Declaring an empty TicTacToe Board
        drawBoard(board);
        cout<<"Enter 1 to give the first move or 2 to give the second move:"<<endl;
        int inp_mv;
        cin>>inp_mv;
        if(inp_mv==1){
            player = 'X';                       //Initializing player as 'X' as the player is giving the first move
            computer = 'O';
            automateBoard1(board);
        }
        else if(inp_mv==2){
            computer = 'X';                     //Initializing computer as 'X' as the computer is giving the first move
            player = 'O';
            board[0][0] = computer;
            cout<<"\nBoard after computer's move:\n";
            drawBoard(board);
            automateBoard2(board);
        }
        else{
            cout<<"Give input 1 or 2 and try again"<<endl;
        }
        cout<<"If you want to PLAY AGAIN enter 'y', to QUIT enter any key :"<<endl;
        cin>>play;                          //play variable takes input whether the player wants to play or not
    }	
	return 0;
}