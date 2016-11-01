// ActivateModern.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "windowsapp")
#include "winrt\base.h"
#include "winrt\Windows.Foundation.h"
#include "winrt\Windows.Web.Syndication.h"
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Syndication;
using namespace std;

IAsyncAction RetrieveFeed()
{
    Uri uri(L"http://www.iamraf.net/Rss");
    SyndicationClient client;
    SyndicationFeed feed = co_await client.RetrieveFeedAsync(uri);
    wcout << feed.Title().Text().c_str() << endl;

    // Bonus! Items too :)
    for (SyndicationItem item : feed.Items())
    {
        hstring title = item.Title().Text();

        wcout << title.c_str() << endl;
    }
}

int main()
{
    initialize();   // RoInitialize

    RetrieveFeed().get();

    cin.get();
    return 0;
}

