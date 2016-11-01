#pragma once

#include <wrl.h>
#include <string>

int PrintError(HRESULT hr);
void TraceApartmentInfo(const std::wstring& prefix);