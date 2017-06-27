#include <stdio.h>
#include <string.h>
#include <locale.h>

int main()
{
	// MBCS
	{
		char str[] = "ABC한글";
		int size = sizeof(str);
		int len = strlen(str);

		printf("배열의 크기: %d \n", size);
		printf("문자열 길이: %d \n", len);
	}

	printf("-------------------------- \n");
	// UNICODE, WBCS
	{

		wchar_t str[] = L"ABC한글";
		int size = sizeof(str);
		int len = wcslen(str);
		_wsetlocale(LC_ALL, L"korean");
		wprintf(L"배열의 크기: %d \n", size);
		wprintf(L"문자열 길이: %d \n", len);
	}

	return 0;
}