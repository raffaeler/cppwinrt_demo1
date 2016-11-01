// InspectModern.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "windowsapp")
#include "winrt\base.h"
#include "winrt\Windows.Foundation.h"
#include "winrt\Windows.Web.Syndication.h"
#include <string>
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Syndication;
using namespace std;

int PrintError(HRESULT hr)
{
    wcout << hex << L"0x" << hr << endl;
    return hr;
}

HRESULT PrintData(const hstring& classId)
{
    HRESULT hr;

    SyndicationClient client;

    hstring name;
    hr = client->abi_GetRuntimeClassName(winrt::put(name));
    if (FAILED(hr)) return PrintError(hr);
    wcout << L"GetRuntimeClassName: " << name.c_str() << endl;

    winrt::Windows::TrustLevel trustLevel;
    hr = client->abi_GetTrustLevel(&trustLevel);
    if (FAILED(hr)) return PrintError(hr);
    wstring trustLevelString;

    switch (trustLevel)
    {
    case winrt::Windows::TrustLevel::BaseTrust:
        trustLevelString = wstring(L"BaseTrust");
        break;
    case winrt::Windows::TrustLevel::PartialTrust:
        trustLevelString = wstring(L"PartialTrust");
        break;
    case winrt::Windows::TrustLevel::FullTrust:
        trustLevelString = wstring(L"FullTrust");
        break;
    default:
        trustLevelString = wstring(L"Unknown");
    }
    wcout << L"GetTrustLevel: " << trustLevelString.c_str() << endl;

    uint32_t count;
    LPIID piid;
    hr = client->abi_GetIids(&count, &piid);
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
}


int main()
{
    initialize();   // RoInitialize

	PrintData(L"Windows.Web.Syndication.SyndicationClient");

	//wcout << endl << endl;
	//PrintData(L"Windows.Data.Json.JsonValue");

    cin.get();
    return 0;
}

