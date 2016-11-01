#pragma once

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.ApplicationModel.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.Web.Syndication.h>

#include <iostream>
#include <string>

namespace winrt
{
    //using namespace winrt;
    using namespace Windows::ApplicationModel;
    using namespace Windows::ApplicationModel::Activation;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Web::Syndication;
}

void CXtoWinrt()
{
    using namespace std;

    Windows::Web::Syndication::SyndicationClient^ cxclient = ref new Windows::Web::Syndication::SyndicationClient();
    cxclient->Timeout = 25000;
    auto winrtAbi = reinterpret_cast<winrt::ABI::Windows::Web::Syndication::ISyndicationClient*>(cxclient);

    winrt::SyndicationClient client(nullptr);    // empty container
    //client.Timeout(12000);    // Of course can't do this
    winrt::copy_from(client, winrtAbi); // attach + addref

    wcout << client.Timeout() << endl;
}

