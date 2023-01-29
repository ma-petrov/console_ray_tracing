#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int getkey() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getc(stdin);
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}