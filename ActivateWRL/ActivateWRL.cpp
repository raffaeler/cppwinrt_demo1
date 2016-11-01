#define __WRL_STRICT__
#pragma comment(lib, "runtimeobject.lib")


#include "stdafx.h"
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

#include <windows.web.syndication.h>
#include <windows.web.atompub.h>
#include <windows.data.json.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;

#include <iostream>
using namespace std;

int PrintError(HRESULT hr)
{
    wcout << hex << L"0x" << hr << endl;
    return hr;
}

ComPtr<IUriRuntimeClass> GetUri(wstring address)
{
    HRESULT hr;

    ComPtr<IActivationFactory> spFactory;
    ComPtr<IUriRuntimeClassFactory> spUriFactory;
    hr = GetActivationFactory(
        HString::MakeReference(RuntimeClass_Windows_Foundation_Uri).Get(),
        &spFactory);

    if (FAILED(hr))
        return ComPtr<ABI::Windows::Foundation::IUriRuntimeClass>();

    hr = spFactory.As(&spUriFactory);

    // create the string
    HString hstrAddress;
    hstrAddress.Set(address.c_str());

    // create the uri
    ComPtr<IUriRuntimeClass> spUri;
    hr = spUriFactory->CreateUri(hstrAddress.Get(), &spUri);
    if (FAILED(hr))
        return ComPtr<IUriRuntimeClass>();
    return spUri;
}

HRESULT PrintTimeout()
{
    HRESULT hr = S_OK;
    auto spUri = GetUri(L"http://www.iamraf.net/Rss");

    // direct activation via RoActivateInstance
    ComPtr<ABI::Windows::Web::Syndication::ISyndicationClient> spClient;
    hr = RoActivateInstance(
        HStringReference(RuntimeClass_Windows_Web_Syndication_SyndicationClient).Get(),
        &spClient);

    if (FAILED(hr))
        return PrintError(hr);

    UINT32 timeout;
    hr = spClient->get_Timeout(&timeout);
    if (FAILED(hr))
        return PrintError(hr);

    wcout << L"Default timeout: " << timeout << endl;

    return S_OK;
}

int main()
{
    RoInitializeWrapper init(RO_INIT_MULTITHREADED);
    if (FAILED(init))
        return PrintError(init);

    PrintTimeout();

    wcout << L"Press any key to exit" << endl;
    cin.get();
    return 0;
}
