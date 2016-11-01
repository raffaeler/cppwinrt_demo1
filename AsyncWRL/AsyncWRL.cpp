#define __WRL_STRICT__
#pragma comment(lib, "runtimeobject.lib")


#include "stdafx.h"
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/client.h>
#include <wrl/event.h>
#include "Utilities.h"

#include <windows.web.syndication.h>
#include <windows.web.atompub.h>
#include <windows.data.json.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Web::Syndication;

#include <windows.storage.h>
using namespace ABI::Windows::Storage;

#include <iostream>
using namespace std;


ComPtr<ABI::Windows::Foundation::IUriRuntimeClass> GetUri(wstring address)
{
    HRESULT hr;

    ComPtr<IActivationFactory> spFactory;
    ComPtr<ABI::Windows::Foundation::IUriRuntimeClassFactory> spUriFactory;
    hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Foundation_Uri).Get(), &spFactory);
    if (FAILED(hr))
        return ComPtr<ABI::Windows::Foundation::IUriRuntimeClass>();
    hr = spFactory.As(&spUriFactory);

    // create the string
    HString hstrAddress;
    hstrAddress.Set(address.c_str());

    // create the uri
    ComPtr<ABI::Windows::Foundation::IUriRuntimeClass> spUri;
    hr = spUriFactory->CreateUri(hstrAddress.Get(), &spUri);
    if (FAILED(hr))
        return ComPtr<ABI::Windows::Foundation::IUriRuntimeClass>();
    return spUri;
}

HRESULT MakRSSRequest()
{
    HRESULT hr = S_OK;
    auto spUri = GetUri(L"http://www.iamraf.net/Rss");

    ComPtr<ISyndicationCategory> spSyndacationCategory;
    hr = RoActivateInstance(
        HStringReference(RuntimeClass_Windows_Web_Syndication_SyndicationCategory)
        .Get(), &spSyndacationCategory);

    if (FAILED(hr))
        return PrintError(hr);

    ComPtr<ISyndicationClient> spClient;
    hr = RoActivateInstance(HStringReference(RuntimeClass_Windows_Web_Syndication_SyndicationClient).Get(), &spClient);
    if(FAILED(hr))
    	return PrintError(hr);

    ABI::Windows::Foundation::__FIAsyncOperationWithProgress_2_Windows__CWeb__CSyndication__CSyndicationFeed_Windows__CWeb__CSyndication__CRetrievalProgress_t *progress;
    hr = spClient->RetrieveFeedAsync(spUri.Get(), &progress);
    if(FAILED(hr))
    	return PrintError(hr);

    auto feedCallback = Callback<
        ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<
        SyndicationFeed*, RetrievalProgress>>(
            [&](ABI::Windows::Foundation::IAsyncOperationWithProgress<
                SyndicationFeed*, RetrievalProgress> *pOperation,
                AsyncStatus status) -> HRESULT
    {
        ComPtr<ISyndicationFeed> spFeed;
        auto hr = pOperation->GetResults(&spFeed);
        if (FAILED(hr))
            return PrintError(hr);

        ComPtr<ISyndicationText> spText;
        hr = spFeed->get_Title(&spText);
        if (FAILED(hr))
            return PrintError(hr);

        HString hstr;
        uint32_t len;
        hr = spText->get_Text(hstr.GetAddressOf());
        if (FAILED(hr))
            return PrintError(hr);

        wcout << hstr.GetRawBuffer(&len) << endl;

        return S_OK;
    });

    progress->put_Completed(feedCallback.Get());
    return S_OK;
}

int main()
{
    RoInitializeWrapper init(RO_INIT_MULTITHREADED);
    if (FAILED(init))
        return PrintError(init);

    MakRSSRequest();

    wcout << L"Press any key to exit" << endl;
    cin.get();
    return 0;
}
