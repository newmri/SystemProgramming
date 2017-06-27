#include <stdio.h>
#include <locale.h>

int main()
{
	// MBCS
	{
		char str[] = "한글입니다";
		int i;

		for (i = 0; i < 5; ++i) fputc(str[i], stdout);

		fputs("\n", stdout);

		for (i = 0; i < 10; ++i) fputc(str[i], stdout);

		fputs("\n", stdout);

	}

	// WBCS
	{

		wchar_t str[] = L"한글입니다";
		int i;

		_wsetlocale(LC_ALL, L"korean");

		for (i = 0; i < 5; ++i) fputwc(str[i], stdout);

		fputws(L"\n", stdout);


	}
	return 0;
}