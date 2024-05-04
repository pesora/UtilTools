// ema_test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ema.h"


double calculate_EMA(double data[], int data_count, double alpha) {
    if (alpha <= 0 || alpha >= 1) {
        printf("Error: The smoothing factor alpha must be between 0 and 1.\n");
        exit(1);
    }

    double prevEmaData = data[0]; // 첫 번째 데이터로 EMA 초기화

    double EMA = data[0]; // 첫 번째 데이터로 EMA 초기화

    for (int i = 1; i < data_count; i++) {
        EMA = (alpha * data[i]) + ((1 - alpha) * prevEmaData);
        prevEmaData = EMA;

        printf("%.2f ", EMA);
    }
    printf("\n");
    return EMA;
}

int main()
{
    //double data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; // 시계열 데이터
    //int size = sizeof(data) / sizeof(data[0]); // 데이터 개수
    //double alpha = 0.1; // 스무딩 팩터 (0 < alpha < 1)


    double data[] = { 17475, 17556, 16953, 18822, 20888, 16943, 19818, 17190, 17475, 17556, 16953,  18822, 20888, 16943, 19818, 17190 }; // 시계열 데이터
    int size = sizeof(data) / sizeof(data[0]); // 데이터 개수
 
    int n = size;
    double alpha = 2.0 / (double)(n +1);

    double EMA = calculate_EMA(data, size, alpha);

    printf("The Exponential Moving Average is: %.2f\n", EMA);

    return 0;

}

