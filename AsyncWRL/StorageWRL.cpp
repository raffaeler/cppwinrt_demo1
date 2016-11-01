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

HRESULT WriteFileInternal(const ComPtr<IStorageFile>& spStorageFile, const wstring& str)
{
    TraceApartmentInfo(L"WriteFileInternal");

    HRESULT hr;
    ComPtr<IActivationFactory> spFactory;
    hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Storage_FileIO).Get(), &spFactory);
    if (FAILED(hr)) return PrintError(hr);
    ComPtr<IFileIOStatics> spFileIOStatics;
    hr = spFactory.As<IFileIOStatics>(&spFileIOStatics);
    if (FAILED(hr)) return PrintError(hr);
    ABI::Windows::Foundation::IAsyncAction *pWriteOperation;
    HSTRING hString;
    WindowsCreateString(str.c_str(), str.size(), &hString);
    HString wrlStr;
    wrlStr.Attach(hString);
    hr = spFileIOStatics->WriteTextAsync(spStorageFile.Get(), wrlStr.Get(), &pWriteOperation);
    if (FAILED(hr)) return PrintError(hr);
    auto cb = Callback<ABI::Windows::Foundation::IAsyncActionCompletedHandler>([&](ABI::Windows::Foundation::IAsyncAction *pAction, AsyncStatus status) -> HRESULT
    {
        switch (status)
        {
        case AsyncStatus::Started:
            TraceApartmentInfo(L"Started");
            break;
        case AsyncStatus::Canceled:
            TraceApartmentInfo(L"Canceled");
            break;
        case AsyncStatus::Error:
            TraceApartmentInfo(L"Error");
            break;
        case AsyncStatus::Completed:
            TraceApartmentInfo(L"Completed");
            ComPtr<IStorageFile> spStorageFile;
            //HRESULT hr = pAction->GetResults();
            if (FAILED(hr)) return PrintError(hr);
            break;
        }
        return S_OK;
    });

    pWriteOperation->put_Completed(cb.Get());
    return S_OK;
}

HRESULT CreateStorageFile(const wstring& filename, const wstring& content, __FIAsyncOperation_1_Windows__CStorage__CStorageFile **pCreateOperation)
{
    HRESULT hr;

    ComPtr<IActivationFactory> spFactory;

    hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Storage_ApplicationData).Get(), &spFactory);
    if (FAILED(hr)) return PrintError(hr);

    ComPtr<IApplicationDataStatics> spApplicationDataStatics;
    hr = spFactory.As<IApplicationDataStatics>(&spApplicationDataStatics);
    if (FAILED(hr)) return PrintError(hr);

    ComPtr<IApplicationData> spApplicationData;
    hr = spApplicationDataStatics->get_Current(&spApplicationData);
    if (FAILED(hr)) return PrintError(hr);

    ComPtr<IStorageFolder> spStorageFolder;
    hr = spApplicationData->get_LocalFolder(&spStorageFolder);
    if (FAILED(hr)) return PrintError(hr);

    HSTRING_HEADER header;
    HSTRING hstr;
    WindowsCreateStringReference(filename.c_str(), filename.length(), &header, &hstr);

    //__FIAsyncOperation_1_Windows__CStorage__CStorageFile *pCreateOperation;
    spStorageFolder->CreateFileAsync(hstr, CreationCollisionOption::CreationCollisionOption_ReplaceExisting, pCreateOperation);
    auto cb = Callback<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<StorageFile*>>([&]
    (ABI::Windows::Foundation::IAsyncOperation<StorageFile*> *pOperation, AsyncStatus status) -> HRESULT
    {
        switch (status)
        {
        case AsyncStatus::Started:
            TraceApartmentInfo(L"Started");
            break;

        case AsyncStatus::Canceled:
            TraceApartmentInfo(L"Canceled");
            return E_FAIL;

        case AsyncStatus::Error:
            TraceApartmentInfo(L"Error");
            return E_FAIL;

        case AsyncStatus::Completed:
            TraceApartmentInfo(L"Completed");
            ComPtr<IStorageFile> spStorageFile;
            HRESULT hr = pOperation->GetResults(&spStorageFile);
            if (FAILED(hr)) return PrintError(hr);

            WriteFileInternal(spStorageFile, content);
            wcout << L"Write done" << endl;
            break;
        }
        return S_OK;
    });

    hr = (*pCreateOperation)->put_Completed(cb.Get());
    if (FAILED(hr)) return PrintError(hr);

    return S_OK;
}

int main2()
{
    RoInitializeWrapper init(RO_INIT_MULTITHREADED);
    if (FAILED(init))
        return PrintError(init);

    __FIAsyncOperation_1_Windows__CStorage__CStorageFile *pCreateOperation;
    auto result = CreateStorageFile(L"c:\\test.xyz", L"Hello, WorldAsync", &pCreateOperation);

    wcout << L"Press any key to exit" << endl;
    cin.get();
    return 0;
}
