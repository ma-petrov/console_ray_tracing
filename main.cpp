#include <iostream>
#include <unistd.h>
#include <queue>
#include <thread>

#include "config.cpp"
#include "vec.cpp"
#include "input.cpp"

using namespace std;

bool is_quit = false;
queue<char> input_queue;
vec3 ro = vec3(-10, 0, 0);
float camera_rotation = 0;

vec3 light = norm(vec3(-0.5, 0.5, -0.5));

char render_pixel(vec2 uv, vec3 ro, vec3 rd) {
    float diff = 0;
    float min_it = 99999;
    
    vec3 n = 0;
    vec2 intersection = vec2(0, 0);
    
    vec3 sphere_pos = vec3(2, 2, 0);
    intersection = sphere(ro - sphere_pos, rd, 1);
    if (intersection.x > 0) {
        vec3 it_point = ro - sphere_pos + rd * intersection.x;
        min_it = intersection.x;
        n = norm(it_point);
    }

    sphere_pos = vec3(2, -2, 0);
    intersection = sphere(ro - sphere_pos, rd, 1);
    if (intersection.x > 0) {
        vec3 it_point = ro - sphere_pos + rd * intersection.x;
        if (intersection.x < min_it) {
            min_it = intersection.x;
            n = norm(it_point);
        }
    }

    if (min_it < MAX_IT) {
        diff = clamp(dot_product(n, light) * 20, 1, COLOR_SIZE - 1);
        return COLOR[(int)diff];
    }
    else return COLOR[0];
}

void render(char** frame) {
    // light = rotateZ(light, 0.01);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            vec2 uv = calculate_uv(j, i);
			vec3 rd = norm(vec3(2, uv));
            rd = rotateZ(rd, camera_rotation);
            frame[i][j] = render_pixel(uv, ro, rd);
        }
    }
}

void display(char** frame) {
    printf("\x1B[2J\x1B[H");
    if (is_quit) {
        puts("\n\n\n\n\n\n            quitting, press any key");
    }
    else {
        puts("camera position:");
        printf("x: %f, y: %f, z: %f\n", ro.x, ro.y, ro.z);
        for (int i = 0; i < HEIGHT; i++) puts(frame[i]);
    }
}

void read_input() {
    int i = 0;
    char c;
    while (!is_quit) {
        c = getkey();
        input_queue.push(c);
    }
}

void move_camera(queue<char>* input_queue) {
    if (!input_queue->empty()) {
        char c = input_queue->front();
        if (c == 'w') ro = ro + rotateZ(vec3(1, 0, 0), camera_rotation);
        if (c == 's') ro = ro - rotateZ(vec3(1, 0, 0), camera_rotation);
        if (c == 'a') ro = ro - rotateZ(vec3(1, 0, 0), camera_rotation + M_PI/2);
        if (c == 'd') ro = ro + rotateZ(vec3(1, 0, 0), camera_rotation + M_PI/2);
        if (c == 'j') camera_rotation -= 0.05;
        if (c == 'k') camera_rotation += 0.05;
        if (c == '\x1b') is_quit = true;
        input_queue->pop();
    }
}

void start_rendering() {
    char** frame = new char*[HEIGHT];
    for (int i = 0; i < HEIGHT; i++) frame[i] = new char[WIDTH];

    while (!is_quit) {
        move_camera(&input_queue);
        usleep(10000);
        render(frame);
        display(frame);
    }
}


int main() {
    thread start_rendering_thread(start_rendering);
    read_input();
}