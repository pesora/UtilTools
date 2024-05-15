// BinToBCDTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <stdio.h>

unsigned int binaryToBCD(unsigned int binary) {
    unsigned int bcd = 0;
    for (int shift = 0; binary != 0; binary >>= 4, shift += 2)
        bcd += (binary & 0xf) << shift;
    return bcd;
}

unsigned int dec2bcd(unsigned int num)
{
    unsigned int ones = 0;
    unsigned int tens = 0;
    unsigned int temp = 0;

    ones = num % 10;
    temp = num / 10;
    tens = temp << 4;
    return (tens + ones);
}
/*
* Print binary from decimal
------------------------------
unsigned short b = 64;

//0000 0000 0100 0000
//1 & 0000 0000 0000 0001

for (int i = 15; i > -1; i--)
{
    printf("%d ", (b >> i) & 0x0001);
}
*/

int main() {
    unsigned char binary = 0;
    unsigned char bcd = 0;

    for (int i = 0; i < 100; i++)
    {
        bcd = dec2bcd(binary); //binaryToBCD(binary);

        printf("Hex 0x%X(%d) >> BCD: %u >> ", binary, binary, bcd);
        for (int i = 7; i > -1; i--)
        {
            printf("%d", (bcd >> i) & 0x01);
        }
        printf("\n");

        binary++;
    }
    return 0;
}