#define __WRL_STRICT__
#pragma comment(lib, "runtimeobject.lib")


#include "stdafx.h"
#include <wrl/wrappers/corewrappers.h>
#include <wrl/client.h>

#include <windows.web.syndication.h>
#include <windows.data.json.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace Windows::Foundation;


#include <iostream>
using namespace std;

int PrintError(HRESULT hr)
{
    wcout << hex << L"0x" << hr << endl;
    return hr;
}

HRESULT PrintData(HStringReference classId)
{
    HRESULT hr;
    //// create the factory for the SyndacationClient object
    //ComPtr<IActivationFactory> spActivationFactory;
    //hr = GetActivationFactory(classId.Get(), &spActivationFactory);
    //if(FAILED(hr)) return PrintError(hr);

    //// use the factory to create the SyndacationClient object and return an IInspectable interface
    //ComPtr<IInspectable> spInspectable;
    //hr = spActivationFactory->ActivateInstance(&spInspectable);
    //if(FAILED(hr)) return PrintError(hr);


    // otherwise create the SyndacationClient object directly
    ComPtr<IInspectable> spInspectable;
    hr = ActivateInstance(classId.Get(), &spInspectable);
    if (FAILED(hr)) return PrintError(hr);

    spInspectable.Get()->AddRef();
    long current = spInspectable.Get()->Release();


    ComPtr<IInspectable> spInspectable2;
    spInspectable2 = spInspectable;

    spInspectable.Get()->AddRef();
    current = spInspectable.Get()->Release();

    // get the class name and print it on the console
    HString runtimeClassName;
    hr = spInspectable->GetRuntimeClassName(runtimeClassName.GetAddressOf());
    if (FAILED(hr)) return PrintError(hr);
    wcout << L"GetRuntimeClassName: " << WindowsGetStringRawBuffer(runtimeClassName.Get(), nullptr) << endl;

    // get the trust level and print it on the console
    TrustLevel trustLevel;
    hr = spInspectable->GetTrustLevel(&trustLevel);
    if (FAILED(hr)) return PrintError(hr);
    wstring trustLevelString;
    switch (trustLevel)
    {
    case TrustLevel::BaseTrust:
        trustLevelString = wstring(L"BaseTrust");
        break;
    case TrustLevel::PartialTrust:
        trustLevelString = wstring(L"PartialTrust");
        break;
    case TrustLevel::FullTrust:
        trustLevelString = wstring(L"FullTrust");
        break;
    default:
        trustLevelString = wstring(L"Unknown");
    }
    wcout << L"GetTrustLevel: " << trustLevelString.c_str() << endl;

    // load the IID list
    ULONG count;
    LPIID piid;
    hr = spInspectable->GetIids(&count, &piid);
    if (FAILED(hr)) return PrintError(hr);
    LPIID temp = piid;

    // loop the IIDs
    for (ULONG i = 0; i<count; i++)
    {
        // get a string version of the IID and print it on the console
        LPOLESTR str;
        if (FAILED(StringFromIID(*temp, &str)))
            break;
        wcout << L"IID " << i << L": " << str << endl;

        // free the olestr
        CoTaskMemFree(str);

        //// now try to queryinterface the object with the current IID
        //ComPtr<IUnknown> spIID;
        //hr = spInspectable.AsIID(*temp, &spIID);
        //if (FAILED(hr)) return PrintError(hr);

        temp++;	// next IID
    }

    // free the IIDs list
    CoTaskMemFree(piid);
    return S_OK;
}


int _tmain(int argc, _TCHAR* argv[])
{
    RoInitializeWrapper init(RO_INIT_MULTITHREADED);
    if (FAILED((HRESULT)init)) return PrintError((HRESULT)init);

    PrintData(HString::MakeReference(
        RuntimeClass_Windows_Web_Syndication_SyndicationClient));

    wcout << endl << endl;
    PrintData(HString::MakeReference(RuntimeClass_Windows_Data_Json_JsonValue));

    wcout << L"Press enter to exit" << endl;
    cin.get();
    return 0;
}

