#include <stdio.h>
int main() {
	//数组声明
	int numbers[5];
	
	//从标准输入（控制台）读取输入
	for(int i = 0;i < 5; i++)
	    scanf("%d", &numbers[i]);
	
	//计算总和
    int result = 0;
	for(int i=0; i < 5; i++)
	    result += numbers[i];
    printf("sum:%d\n", result);
	return 0;
}
