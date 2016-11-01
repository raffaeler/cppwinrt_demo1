// ActivateModern.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "windowsapp")
#include "winrt\base.h"
#include "winrt\Windows.Foundation.h"
#include "winrt\Windows.Web.Syndication.h"
//#include <string>
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Syndication;
using namespace std;

HRESULT PrintTimeout()
{
    Uri uri(L"http://www.iamraf.net/Rss");
    SyndicationClient client;

    wcout << L"Default timeout: " << client.Timeout() << endl;

    return S_OK;
}

int main()
{
    initialize();   // RoInitialize

    PrintTimeout();

    cin.get();
    return 0;
}

