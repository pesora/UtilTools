#include <stdio.h>
#include <stdlib.h>

double calculate_EMA(double data[], int data_count, double alpha) {
    if (alpha <= 0 || alpha >= 1) {
        printf("Error: The smoothing factor alpha must be between 0 and 1.\n");
        exit(1);
    }

    double EMA = data[0]; // 첫 번째 데이터로 EMA 초기화

    for(int i = 1; i < data_count; i++) {
        EMA = (alpha * data[i]) + ((1 - alpha) * EMA);
    }

    return EMA;
}

int main() {
    double data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // 시계열 데이터
    int data_count = sizeof(data) / sizeof(data[0]); // 데이터 개수
    double alpha = 0.1; // 스무딩 팩터 (0 < alpha < 1)

    double EMA = calculate_EMA(data, data_count, alpha);

    printf("The Exponential Moving Average is: %.2f\n", EMA);

    return 0;
}