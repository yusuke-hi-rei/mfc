//! Operation test environment: Windows 10 English 
//! OS Region:                  United state
//! Development environment :   Visual Studio 2019(English)
//!                             MFC Console application.
//! Used files(test.txt) :      Windows 10 Japanese created.
//!                             UTF-8(Non BOM)

//! The period I spent before I realized this was two days.
//! The reason why this was troublesome is that if you type L"‚ ‚¢‚¤" directly into the source code and run it,
//! you can display it in wcout << L"‚ ‚¢‚¤".
//! This is because if you type it directly,
//! it will be a three-byte character.(debug watch)

#include <iostream>
#include <clocale>
#include <cstdlib>

int main() {
	//! Set the code page for the console.
	//! The console can only be configured in UTF-8 or UTF-7.
	SetConsoleOutputCP(CP_UTF8);
	//! Sets the locale of wcout to the current locale.
	wcout.imbue(std::locale("", std::locale::ctype));

	//! Initialize wchar and char.
	wchar_t wideString[256];
	SecureZeroMemory(wideString, sizeof(wideString));
	char mbString[256];
	SecureZeroMemory(mbString, sizeof(mbString));
	//! Open a dummy file with Japanese. [kana: ‚ ‚¢‚¤‚¦‚¨ ]
	CString filePathTest = L"C:\\Test\\test.txt";
	FILE* pStream = _wfopen(filePathTest, L"ccs=UTF-8");
	//! WCHAR is read from a file in UTF-16 format.
	//! Therefore, if you try to display a three-byte character (hiragana) in the console,
	//! the console does not support that character.
	fgetws(wideString, _countof(wideString) - 1, pStream);
	//! Converts WCHAR to multibyte characters to match the format of 3-byte characters.
	{
		//! pattern1
		int size = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, mbString, 256, NULL, NULL);
	}
	// or
	{
		//! pattern2
		//! The reason for running setlocale is that wcstombs() returns a size of 0 when an unrecognized character is entered.
		std::setlocale(LC_CTYPE, "en_US.UTF-8");
		wcstombs(mbString, wideString, sizeof(mbString));
		//! The reason for running setlocale ("C") is that japanese characters are not displayed on the console unless you revert to the initial locale.
		std::setlocale(LC_CTYPE, "C");
	}
	//! Prints multibyte characters to the console.
	printf(mbString);
	return 0;
}