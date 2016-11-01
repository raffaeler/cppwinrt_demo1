#pragma once

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.ApplicationModel.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.Web.Syndication.h>

#include <iostream>
#include <string>

template <typename CX, typename Trt>
CX copyCX(const Trt& rt)
{
    auto abi = winrt::get(rt);
    auto cx = reinterpret_cast<CX>(abi);
    return cx;
}

void WinrttoCX()
{
    using namespace std;

    winrt::Windows::Web::Syndication::SyndicationClient client;
    client.Timeout(25000);

    auto cx = copyCX<Windows::Web::Syndication::SyndicationClient^>(client);

    wcout << cx->Timeout << endl;
}