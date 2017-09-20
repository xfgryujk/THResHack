#include "stdafx.h"
#include <string>
using namespace std;


class DllHijack final
{
public:
	HMODULE m_dinput8Module = NULL;

	typedef HRESULT(WINAPI* DirectInput8CreateType)(HINSTANCE hinst, DWORD dwVersion,
		REFIID riidltf, LPVOID *ppvOut, void* punkOuter);
	DirectInput8CreateType m_realDirectInput8Create = NULL;


	DllHijack()
	{
		wstring dllPath(MAX_PATH, L'\0');
		GetSystemDirectoryW(&dllPath.front(), dllPath.size());
		dllPath.resize(wcslen(dllPath.c_str()));
		dllPath += L"\\dinput8.dll";

		m_dinput8Module = LoadLibraryW(dllPath.c_str());
		m_realDirectInput8Create = (DirectInput8CreateType)GetProcAddress(m_dinput8Module, "DirectInput8Create");
	}

	~DllHijack()
	{
		if (m_dinput8Module != NULL)
			FreeLibrary(m_dinput8Module);
	}
};

static DllHijack g_dllHijack;


#pragma comment(linker, "/EXPORT:DirectInput8Create=_MyDirectInput8Create@20")
extern "C" HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion,
	REFIID riidltf, LPVOID *ppvOut, void* punkOuter)
{
	if (g_dllHijack.m_realDirectInput8Create == NULL)
		return E_NOTIMPL;
	return g_dllHijack.m_realDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}
