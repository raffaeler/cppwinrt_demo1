// MixingInterop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

#pragma comment(lib, "windowsapp")
#include <winrt\base.h>

#include "CXtoWinrt1.h"
//#include "CXtoWinrt2.h"
//#include "CXtoWinrt3.h"

#include "WinrttoCX1.h"
//#include "WinrttoCX2.h"
//#include "WinrttoCX3.h"

int main()
{
    winrt::initialize();

    CXtoWinrt();

    WinrttoCX();

    std::cin.get();
    return 0;
}

