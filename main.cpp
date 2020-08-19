#include <iostream>
#include <time.h>
#include <ncurses.h>
#include <cstdlib>

using namespace std;
void setup(){
    
    initscr();// initscr()函数将终端屏幕初始化为curses 模式
    clear();
    noecho(); //禁止字符出现在终端上
    cbreak();//可以禁止行缓冲
    curs_set(0);//这个函数用来设制光标是否可见
    
    
}

enum eDir{STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6};

class cBall{
private:
    int x, y;
    int OriginalX, OriginalY;
    eDir direction;
public:
    cBall(int posX, int posY){
        OriginalX = posX;
        OriginalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
    }

    void Reset(){
        x = OriginalX;
        y = OriginalY;
        direction = STOP;
    }

    void changeDirection(eDir d){
        direction = d;
    }

    inline int getX(){
        return x;
    }

    inline int getY(){
        return y;
    }

    inline eDir getDirection(){
        return direction;
    }

    void randomDirection(){
        direction = (eDir)((rand() % 6) + 1);

    }

    void Move(){
        switch (direction){
            case STOP:
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UPLEFT:
                x--; y--;
                break;
            case DOWNLEFT:
                x--; y++;
                break;
            case UPRIGHT:
                x++; y--;
                break;
            case DOWNRIGHT:
                x++; y++;
                break;
            default:
                break;
        }
    }

    friend ostream & operator<< (ostream & o, cBall c);

};


class cPaddle{
private:
    int x,y;
    int originalX, originalY;
public:

    cPaddle(){
        x= y =0;
    }

    cPaddle(int posX, int posY){
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
    }

    inline void Reset(){
        x = originalX;
        y = originalY;
    }

    inline int getX(){
        return x;
    }

    inline int getY(){
        return y;
    }

    inline void moveUP(){
        y--;
    }
    inline void moveDOWN(){
        y++;
    }
    friend ostream & operator<< (ostream & o, cPaddle c);


};

class cGameManager{
private:
    int width, height;
    int score1, score2;
    char up1, down1, up2, down2;
    bool quit;
    cBall * ball;
    cPaddle * player1;
    cPaddle * player2;
public:
    cGameManager(int w, int h){
        srand(time(NULL));
        quit = false;
        up1 = 'w'; up2 = 'i';
        down1 = 's'; down2 = 'k';
        score1 = score2 = 0;
        width = w;
        height = h;
        ball = new cBall(w/2, h/2);
        player1 = new cPaddle(2, h/2 - 3);
        player2 = new cPaddle(w - 2, h/2 - 3);

    }
    ~cGameManager(){
        delete ball, player1, player2;
    }

    void ScoreUP(cPaddle * player){
        if (player == player1){
            score1++;
        } else if (player == player2){
            score2++;
        }
        ball ->Reset();
        player1 ->Reset();
        player2 ->Reset();
    }

    void Draw(){

        clear();

        for (int i = 0; i < height + 2; i++){
            for(int j = 0; j < width + 2; j++){
                int ballx = ball -> getX();
                int bally = ball -> getY();
                int player1x = player1 -> getX();
                int player1y = player1 -> getY();
                int player2x = player2 -> getX();
                int player2y = player2 -> getY();

                if (i == 0 || i == height + 1){
                    mvprintw(i, j,"\231");
                } else if (j == 0 || j == width + 1){
                    mvprintw(i, j,"#");
                } else if (i == bally && j == ballx){
                    mvprintw(i, j ,"O");
                } else if (i == player1y && j == player1x){
                    mvprintw(i, j ,"#");
                } else if (i == player2y && j == player2x){
                    mvprintw(i, j ,"#");
                } else if (i == player1y + 1 && j == player1x){
                    mvprintw(i, j ,"#");
                } else if (i == player1y + 2 && j == player1x){
                    mvprintw(i, j ,"#");
                } else if (i == player1y + 3 && j == player1x){
                    mvprintw(i, j ,"#");
                } else if (i == player2y + 1 && j == player2x){
                    mvprintw(i, j ,"#");
                } else if (i == player2y + 2 && j == player2x){
                    mvprintw(i, j ,"#");
                } else if (i == player2y + 3 && j == player2x){
                    mvprintw(i, j ,"#");
                }
                
                
            }
        }
        
        

        mvprintw(height + 5, 0, "Score1 %d", score1);
        mvprintw(height + 6, 0, "Score2 %d", score2);

        refresh();

    }
    
    void input(){
        ball -> Move();
        int ballx = ball -> getX();
        int bally = ball -> getY();
        int player1x = player1 -> getX();
        int player1y = player1 -> getY();
        int player2x = player2 -> getX();
        int player2y = player2 -> getY();
        keypad(stdscr, TRUE);
        halfdelay(5);

        char c = getch();
        
            
        if (c == up1){
            if (player1y > 0){
                player1 -> moveUP();
            }
        }
        
        if (c == up2){
            if (player2y > 0){
                player2 -> moveUP();
            }
        }
        
        if (c == down2){
            if (player2y + 4 < height){
                player2 -> moveDOWN();
            }
        }
        
        if (c == down1){
            if (player1y + 4 < height){
                player1 -> moveDOWN();
            }
        }
            

            
        if (ball -> getDirection() == STOP){
            ball -> randomDirection();
        }
        
        if (c == 'q'){
            quit = true;
        }
            


        
    }
    
    void Logic(){
        ball -> Move();
        int ballx = ball -> getX();
        int bally = ball -> getY();
        int player1x = player1 -> getX();
        int player1y = player1 -> getY();
        int player2x = player2 -> getX();
        int player2y = player2 -> getY();
        
        for (int i = 0; i < 4; i++){
            if (ballx == player1x + 1){
                if (bally == player1y + i){
                    ball -> changeDirection((eDir)((rand() % 3) + 4));
                }
            }
        }
        
        for (int i = 0; i < 4; i++){
            if (ballx == player2x - 1){
                if (bally == player2y + i){
                    ball -> changeDirection((eDir)((rand() % 3) + 1));
                }
            }
        }
        
        if (bally == height - 1){
            ball -> changeDirection( ball -> getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
        }
        
        if (bally == 0){
            ball -> changeDirection( ball -> getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        }
        
        if (ballx == width - 1){
            ScoreUP(player1);
        }
        if (ballx == 1){
            ScoreUP(player2);
        }
        
    }
    
    void Run(){
        
        while (!quit){
            Draw();
            input();
            Logic();
        }
        
    }
};

ostream & operator<< (ostream & o, cBall c){
    o << "Ball [" << c.x << "," << c.y <<"][" << c.direction << "]" ;
    return o;
}

ostream & operator<< (ostream & o, cPaddle c){
    o << "Paddle [" << c.x << "," << c.y <<"]" ;
    return o;
}
int main() {
    setup();
    
    cGameManager c = {20, 20};
    c.Run();
    
    getch();
    endwin();

    return 0;
}
