//SNAKE GAME
#include <iostream>
#include <cstdlib>  // for rand()
#include <ctime>    // for seeding rand()

#include <termios.h>  // for _kbhit(), _getch() (LINUX), for WINDOWS - conio.h
#include <unistd.h>  // For read(), STDIN_FILENO
#include <fcntl.h>   // For non-blocking input control

using namespace std;

//depicts the end of the game
bool gamends;
//now, we create the length of the playing field - wight and height
const int width = 30;
const int height = 30;
//head, tail of the snake; fruit variables; score
int x, y, fruitx, fruity, score; //x and y co-ordinate of head and fruit 
int tailx[100]; //x co-ordinate of tail which stores location of all tail parts consecutively
int taily[100]; //y co-ordinate of tail which stores location of all tail parts consecutively
int ntail; //specifies length of the tail
//directions of the snake - either goes left, right, upwards, downwards or stops
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;

// ----------------------------------------------------------
// Custom getch() function
// Purpose: Reads a single character from input without waiting for Enter.
// It's like _getch() in Windows, which is included under the 'conio.h' package
// ----------------------------------------------------------
char getch() {
    char buf = 0;
    struct termios old = {0};

    // Get current terminal settings
    tcgetattr(0, &old);

    // Modify settings to turn off canonical mode (buffered input)
    // and echo (so characters aren't printed on screen)
    old.c_lflag &= ~ICANON; // Turn off line buffering
    old.c_lflag &= ~ECHO;   // Turn off character echoing

    // Apply the new settings immediately
    tcsetattr(0, TCSANOW, &old);

    // Read one character (non-buffered)
    read(0, &buf, 1);

    // Restore original terminal settings
    tcsetattr(0, TCSADRAIN, &old);

    return buf;
}

// ----------------------------------------------------------
// Custom kbhit() function
// Purpose: Checks if a key has been pressed (non-blocking).
// It's like _kbhit() in Windows, which is included under the 'conio.h' package
// Returns: 1 if key is available, 0 otherwise
// ----------------------------------------------------------
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set terminal to non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Try to read a character
    ch = getchar();

    // Restore terminal settings and blocking mode
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        // If a character was read, put it back into input buffer
        ungetc(ch, stdin);
        return 1; // Key was pressed
    }

    return 0; // No key press detected
}

//----------------------------------------------------------
//MAIN CODE FOR THE GAME STARTS
void setup() //intial setup of the game on the ouput console
{
    gamends = false; //initially the game doesn't start nor ends
    dir = STOP; //intially game is stopped
    //the snake stays in the middle of the map
    x = width / 2; 
    y = width / 2;
    //the fruit stays randomly on the map and it's position is limited between 0 and the map's width
    fruitx = rand() % width;
    fruity = rand() % width;
    score = 0; //intially the score is 0
}

void draw()
{
    system("clear"); //clears the screen first by calling the system clear screen command "cls"
    
    //border of the boundary is set by '%'
    for(int i = 0; i < width + 2; i++)
        cout << "%";
    cout << endl;    
    
    for(int i = 0; i < height; i++) // y co-ordinate
    {
        for(int j = 0; j < width; j++) // x co-ordinate
        {
            if(j == 0) //makes sure that boundary is '%'
                cout << "%";
            if(i == y && j == x) //sets the position of the head and tail of snake and prints uppercase O
                cout << "O";
            else if(i == fruity && j == fruitx) //sets the position of the fruit based off of the location as set
                cout << "F";
            else
            {
                bool print = false;
                for(int k = 0; k < ntail; k++)
                {
                    if(tailx[k] == j && taily[k] == i)
                    {
                        cout << "o"; //prints the next part of the tail, lowercase o
                        print = true;
                    }
                }
                if(!print)
                    cout << " "; //space in the middle stays empty
            }
            
            if(j == width - 1) //boundary is '%'
                cout << "%";
        }
        cout << endl;
    }
    
    //border of the boundary is set by '%'
    for(int i = 0; i < width + 2; i++)
        cout << "%";
    cout << endl;
    //prints the updated score at the end always
    cout << "Score: " << score << endl;
}

void input()
{
    //checks if a key is pressed
    if(kbhit())
    {
        switch(getch()) //fetches a character
        {
            case 'w': //when pressed moves UP
                dir = UP;
                break;
                
            case 's': //when pressed moves DOWN
                dir = DOWN;
                break;
            
            case 'a': //when pressed moves RIGHT
                dir = LEFT;
                break;            
            
            case 'd': //when pressed moves LEFT
                dir = RIGHT;
                break; 
                
            case 'x': //when pressed stops the gamends
                dir = STOP;
                gamends = true;
                break;
            
        }
    }
    
}

void logic()
{
    int prevx = tailx[0]; //to remember the previous x co-ordinate of the tail
    int prevy = taily[0]; //to remember the previous y co-ordinate of the tail
    int prev2x, prev2y;
    
    //first tail co-ordinate is set to the intial head location for proper follow-up
    tailx[0] = x;
    taily[0] = y;
    for(int i = 1; i < ntail; i++) //to spawn the tail properly where current co-ordinate is followed by the previous one
    {
        prev2x = tailx[i];
        prev2y = taily[i];
        tailx[i] = prevx;
        taily[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }
    
    switch(dir)
    {
        case LEFT:
            x--; //'x' co-ordinate decreases when we go left
            break;
            
        case RIGHT:
            x++; //'x' co-ordinate decreases when we go left
            break;
            
        case UP:
            y--; //'x' co-ordinate decreases when we go left
            break;
            
        case DOWN:
            y++; //'x' co-ordinate decreases when we go left
            break;
                
        default:
            break;
    }
        
    /*//game terminates if the snake touches the wall
    if(x >= width || y >= height || x < 0 || y < 0)
        gamends = true;*/
    
    //if we don't want to terminate the game when the snake touches the wall and rather continue from the opposite side
    //if this part of the code is active, then the above game ending logic of the snake touching the wall resulting in game termination
    //    must be commented
    if(x >= width)
        x = 0;
    else if(x < 0)
        x = width - 1;
    if(y >= height)
        y = 0;
    else if(y < 0)
        y = height - 1;
    
    //when snake touches it's own tail, game also ENDS
    for(int i = 0; i < ntail; i++)
        if(tailx[i] == x && taily[i] == y)    
            gamends = true;
    
    //when the snake touches the fruit, score increases and the fruit randomly spawns again, also tail length changes
    if(x == fruitx && y == fruity)
    {
        score += 10; //score increases by 10 each time  
        //the fruit stays randomly on the map and it's position is limited between 0 and the map's width
        fruitx = rand() % width;
        fruity = rand() % width;
        ntail++; //size of tail increases when fruit is eaten
    }
}

int main()
{
    setup();
    while(!gamends)
    {
        draw();
        input();
        logic();
    }
    
    return 0;
}
//MAIN CODE FOR THE GAME ENDS
//----------------------------------------------------------