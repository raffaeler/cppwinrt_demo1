#pragma once

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.ApplicationModel.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.Web.Syndication.h>

#include <iostream>
#include <string>

void WinrttoCX()
{
    using namespace winrt;
    using namespace winrt::Windows::ApplicationModel;
    using namespace winrt::Windows::ApplicationModel::Activation;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Web::Syndication;
    using namespace std;

    SyndicationClient client;
    client.Timeout(25000);
    auto abi = winrt::get(client);

    auto cx = reinterpret_cast<::Windows::Web::Syndication::SyndicationClient^>(abi);
    wcout << cx->Timeout << endl;
}