// 大数阶乘存储问题.cpp: 定义控制台应用程序的入口点。
// 1:100位乘法运算
// 2：阶乘 ，本质是乘法运算

// m长的数* n长的数， 结果不会大于 m+n 长
// 方法：模拟小学乘法手算
#include "stdafx.h"
#include "string.h"

int main()
{
	char s1[100];
	char s2[100];
	int bignumber[100];
	int smallnumber[100];
	int shortlen = 0;
	int longlen = 0;
	
	scanf_s("%s", s1, 100);
	scanf_s("%s", s2, 100);

	printf("s1=%s,len=%d\n", s1, strlen(s1));
	printf("s2=%s,len=%d\n", s2, strlen(s2));

	if (strlen(s1) > strlen(s2))
	{
		longlen = strlen(s1);
		shortlen = strlen(s2);
		for (int i = 0; i < longlen; i++)          //将大字符串数字倒序排列存储倒整形数组bignumber中
		{
			bignumber[i] = s1[strlen(s1) - 1 - i] - '0';
		}
		for (int i = 0; i < shortlen; i++)          //将小字符串数字倒序排列存储倒整形数组smallnumber中
		{
		    smallnumber[i] = s2[strlen(s2) - 1 - i] - '0';
		}
	}
	else
	{
		longlen = strlen(s2);
		shortlen = strlen(s1);
		for (int i = 0; i < longlen; i++)          
		{
			bignumber[i] = s2[strlen(s2) - 1 - i] - '0';
		}
		for (int i = 0; i < shortlen; i++)          
		{
			smallnumber[i] = s1[strlen(s1) - 1 - i] - '0';
		}
	}

  //进行乘法运算 
  //外层是小数循环，内层是大数循环，符合正常的乘法计算书写过程
	int stepresult[200];   // 最大的情况位  100位*1位，最大长度101位，给102位足够
	int result[200];       // 100位*100位，最大长度200位
	memset(result, 0, sizeof(result));
	memset(stepresult, 0, sizeof(stepresult));
	int d=0;
	for (int i = 0; i < shortlen; i++)
	{
		//模拟乘法
		for (int j = 0; j < longlen; j++)
		{
			d=bignumber[j] * smallnumber[i];    //对应的乘法，都是10以内的数相乘 结果只有两位数

			stepresult[j+i]+= d % 10;            //对应位的结果加上乘法结果对10的余数，也就是乘法结果的个位
			if (stepresult[j + i] >= 10)
			{
				stepresult[j + i+1] ++;   //进位
				stepresult[j+i] = stepresult[j+i] - 10;  //本位-10
			}

			stepresult[j+i+1] += d / 10;  //后一位= 加上结果对10的倍数，也就是乘法结果的十位
			if (stepresult[j +i+ 1] >= 10)
			{
				stepresult[j + i + 2]++;  //进位
				stepresult[j+i+1] = stepresult[j+i+1] - 10;  //本位-10
			}
		}
		//输出每一步的乘法运算结果
		for (int i = longlen + shortlen; i >= 0; i--)
		{
			if (stepresult[i] != 0)                //直到某位不为0   从后往前找不为0的数，也就是找到不为0的最高位。
			{
				for (int j = i; j >= 0; j--)//打印结果
				{
					printf("%d", stepresult[j]);
				}
				break;
			}
		}
		printf("\n");

		//做加法  result+stepresult 大数加法
		// （m位的数）* （n位的数），结果不会大于m+n位
		for (int i = 0; i <longlen+shortlen; i++)  //循环次数为大的数的位数
		{
			result[i] = result[i] + stepresult[i]; //一位一位加  在倒序排列的情况下，数组中存放的数的位数为：个十百千万，与实际中的写法相反
			if (result[i]>= 10)       //然后就是进位
			{
				result[i + 1]++;
				result[i] -= 10;
			}
		}
		//将中间的乘法结果清0
		memset(stepresult, 0, sizeof(stepresult));
	}

	//输出结果
	printf("\n");
	printf("result is :");
	for (int i = longlen + shortlen; i >= 0; i--)
	{
		if (result[i] != 0)                //直到某位不为0   从后往前找不为0的数，也就是找到不为0的最高位。
		{
			for (int j = i; j >= 0; j--)//打印结果
			{
				printf("%d", result[j]);
			}
			break;
		}
	}
	printf("\n");

    return 0;
}

