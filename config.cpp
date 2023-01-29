int char_length(char* s) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

int MULT = 20; // std is 20
int WIDTH = 5 * MULT;
int HEIGHT = 2 * MULT;
float ASPECT = WIDTH/HEIGHT;
float PIXEL_ASPECT = 0.67;
char* COLOR = " .:!/r(l1Z4H9W8$@";
int COLOR_SIZE = char_length(COLOR);
float MAX_IT = 99999;