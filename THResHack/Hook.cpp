#include "stdafx.h"
#include "Hook.h"


#pragma pack(push)
#pragma pack(1)
struct JmpCode
{
private:
	const BYTE jmpCode = 0xE9;
	uintptr_t relativeAddr;

public:
	JmpCode(void* srcAddr, void* dstAddr)
	{
		SetAddress(srcAddr, dstAddr);
	}

	void SetAddress(void* srcAddr, void* dstAddr)
	{
		relativeAddr = (uintptr_t)dstAddr - (uintptr_t)srcAddr - sizeof(JmpCode);
	}
};
#pragma pack(pop)


InlineHook::InlineHook(void* originalFunction, void* hookFunction, bool enable) :
	m_originalFunction(originalFunction),
	m_hookFunction(hookFunction)
{
	memcpy(m_originalCode, m_originalFunction, sizeof(JmpCode));

	if (enable)
		Enable();
}

InlineHook::~InlineHook()
{
	Disable();
}

bool InlineHook::Enable()
{
	if (m_isEnabled)
		return true;

	DWORD oldProtect, oldProtect2;
	if (!VirtualProtect(m_originalFunction, sizeof(JmpCode), PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;

	JmpCode code(m_originalFunction, m_hookFunction);
	memcpy(m_originalFunction, &code, sizeof(JmpCode));

	VirtualProtect(m_originalFunction, sizeof(JmpCode), oldProtect, &oldProtect2);

	m_isEnabled = true;
	return true;
}

bool InlineHook::Disable()
{
	if (!m_isEnabled)
		return true;

	DWORD oldProtect, oldProtect2;
	if (!VirtualProtect(m_originalFunction, sizeof(JmpCode), PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;

	memcpy(m_originalFunction, m_originalCode, sizeof(JmpCode));

	VirtualProtect(m_originalFunction, sizeof(JmpCode), oldProtect, &oldProtect2);

	m_isEnabled = false;
	return true;
}
