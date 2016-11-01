#pragma once

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.ApplicationModel.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.Web.Syndication.h>

#include <iostream>
#include <string>

void CXtoWinrt()
{
    using namespace winrt;
    using namespace winrt::Windows::ApplicationModel;
    using namespace winrt::Windows::ApplicationModel::Activation;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Web::Syndication;
    using namespace std;

    ::Windows::Web::Syndication::SyndicationClient^ cxclient = ref new ::Windows::Web::Syndication::SyndicationClient();
    cxclient->Timeout = 25000;
    auto winrtAbi = reinterpret_cast<winrt::ABI::Windows::Web::Syndication::ISyndicationClient*>(cxclient);

    SyndicationClient client(nullptr);    // empty container
    //client.Timeout(12000);    // Of course can't do this
    copy_from(client, winrtAbi); // attach + addref

    wcout << client.Timeout() << endl;
}