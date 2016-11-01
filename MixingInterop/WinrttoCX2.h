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

void WinrttoCX()
{
    using namespace std;

    winrt::SyndicationClient client;
    client.Timeout(25000);
    auto abi = winrt::get(client);

    auto cx = reinterpret_cast<Windows::Web::Syndication::SyndicationClient^>(abi);
    wcout << cx->Timeout << endl;
}
