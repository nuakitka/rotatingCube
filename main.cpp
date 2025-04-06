#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>

float A = 0, B = 0, C = 0;
const float cubeWidth = 10;
const int width = 80, height = 24;
float zBuffer[width * height];
char buffer[width * height];
const char background = ' ';
const float distanceFromCam = 35;
const float K1 = 20;
float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(float i, float j, float k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
        j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(float i, float j, float k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
        j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
        i * cos(B) * sin(C);
}

float calculateZ(float i, float j, float k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForPoint(float i, float j, float k, char ch) {
    x = calculateX(i, j, k);
    y = calculateY(i, j, k);
    z = calculateZ(i, j, k) + distanceFromCam;

    ooz = 1 / z;
    xp = static_cast<int>(width / 2 + K1 * ooz * x);
    yp = static_cast<int>(height / 2 - K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    std::cout << "\x1b[2J";
    while (true) {
        std::memset(buffer, background, width * height);
        std::memset(zBuffer, 0, width * height * sizeof(float));

        // Рисуем 12 рёбер куба
        for (float t = -cubeWidth; t <= cubeWidth; t += 0.5f) {
            // Передняя грань
            calculateForPoint(t, -cubeWidth, -cubeWidth, '#');
            calculateForPoint(t, cubeWidth, -cubeWidth, '#');
            calculateForPoint(-cubeWidth, t, -cubeWidth, '@');
            calculateForPoint(cubeWidth, t, -cubeWidth, '@');

            // Задняя грань
            calculateForPoint(t, -cubeWidth, cubeWidth, '#');
            calculateForPoint(t, cubeWidth, cubeWidth, '#');
            calculateForPoint(-cubeWidth, t, cubeWidth, '@');
            calculateForPoint(cubeWidth, t, cubeWidth, '@');

            // Вертикальные рёбра
            calculateForPoint(-cubeWidth, -cubeWidth, t, '$');
            calculateForPoint(-cubeWidth, cubeWidth, t, '$');
            calculateForPoint(cubeWidth, -cubeWidth, t, '$');
            calculateForPoint(cubeWidth, cubeWidth, t, '$');
        }

        std::cout << "\x1b[H";
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}