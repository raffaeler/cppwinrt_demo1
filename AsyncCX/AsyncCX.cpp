// AsyncCX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include <collection.h>
#include <ppltasks.h>
#include <pplawait.h>

#include <string>
#include <iostream>
using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::Web::Syndication;
using namespace Windows::Web::AtomPub;


concurrency::task<Platform::String^> GetFeedAsync()
{
    Uri ^uri = ref new Uri("http://www.iamraf.net/Rss");    // L is optional in C++/CX
    SyndicationClient^ client = ref new SyndicationClient();
    auto feed = co_await client->RetrieveFeedAsync(uri);
    return feed->Title->Text;
}

int main(Platform::Array<Platform::String^>^ args)
{
    auto title2 = GetFeedAsync().get();
    Platform::Details::Console::WriteLine(title2);

    std::cin.get();
}