#include "stdafx.h"

#include <wrl.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int PrintError(HRESULT hr)
{
    wcout << hex << L"0x" << hr << endl;
    return hr;
}

std::wstring DecodeAptType(APTTYPE AptType)
{
    switch (AptType)
    {
    case APTTYPE::APTTYPE_CURRENT:
        return L"CURRENT";
    case APTTYPE::APTTYPE_STA:
        return L"STA";
    case APTTYPE::APTTYPE_MTA:
        return L"MTA";
    case APTTYPE::APTTYPE_NA:
        return L"NA";
    case APTTYPE::APTTYPE_MAINSTA:
        return L"MAINSTA";
    default:
    {
        std::wstringstream ss;
        ss << L"Unknown (" << (int)AptType << L")";
        return ss.str();
    }
    }
}

std::wstring DecodeAptTypeQualifier(APTTYPEQUALIFIER AptTypeQualifier)
{
    switch (AptTypeQualifier)
    {
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_NONE:
        return L"NONE";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_IMPLICIT_MTA:
        return L"IMPLICIT_MTA";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_NA_ON_MTA:
        return L"NA_ON_MTA";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_NA_ON_STA:
        return L"NA_ON_STA";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_NA_ON_IMPLICIT_MTA:
        return L"NA_ON_IMPLICIT_MTA";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_NA_ON_MAINSTA:
        return L"NA_ON_MAINSTA";
    case APTTYPEQUALIFIER::APTTYPEQUALIFIER_APPLICATION_STA:
        return L"APPLICATION_STA";
    default:
    {
        std::wstringstream ss;
        ss << L"Unknown (" << (int)AptTypeQualifier << L")";
        return ss.str();
    }
    }
}

void TraceApartmentInfo(const wstring& prefix)
{
    std::wstringstream ss;
    ss << prefix.c_str();

    APTTYPE AptType;
    APTTYPEQUALIFIER AptTypeQualifier;
    auto hr = CoGetApartmentType(&AptType, &AptTypeQualifier);
    UINT64 id;
    auto hr2 = RoGetApartmentIdentifier(&id);
    if (FAILED(hr) || FAILED(hr2))
    {
        OutputDebugString(L"TraceApartmentInfo failed");
        return;
    }

    ss << L"[0x" << std::hex << id << std::dec << L"] " << DecodeAptType(AptType).c_str() << L", " << DecodeAptTypeQualifier(AptTypeQualifier).c_str();
    OutputDebugString(ss.str().c_str());
    OutputDebugString(L"\r\n");
    return;
}

