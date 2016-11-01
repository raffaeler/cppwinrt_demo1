#pragma once

#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.ApplicationModel.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.Web.Syndication.h>

template <typename Trtabi, typename Trt, typename CX>
Trt copyRT(CX cx)
{
    auto abi = reinterpret_cast<Trtabi>(cx);
    Trt res(nullptr);
    winrt::copy_from(res, abi);
    return res;
}

void CXtoWinrt()
{
    using namespace std;
    auto cxclient = ref new Windows::Web::Syndication::SyndicationClient();

    auto rtclient = copyRT<
        winrt::ABI::Windows::Web::Syndication::ISyndicationClient *,
        winrt::Windows::Web::Syndication::SyndicationClient>(cxclient);

    std::wcout << rtclient.Timeout() << std::endl;
}
