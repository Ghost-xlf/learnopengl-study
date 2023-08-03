#include <stdio.h>

#include <math.h>

int prime(int n)
{
  int i;
  for (i = 2; i <= sqrt(n); i++) // 循环2-这个数字之间所有的数字
    if (n % i == 0)
      return 0; // 如果能被整除就证明不是素数
  return 1;
}

int main()
{
  int i, k = 0;
  for (i = 101; i < 200; i += 2) // 循环100~200间的数字 ，偶数必定不是素数，所以单数起始，每次+2减少循环次数
    if (prime(i))                // 判断是否为素数
    {
      printf("%d ", i); // 打印素数
      if (++k % 5 == 0)
        printf("\n"); // 如果已经打印五个了打印一次换行
    }
  printf("\n");
  return 0;
}