#pragma once
#include "ResHackBase.h"


class ResHackFastcall final : public ResHackBase
{
protected:
	typedef void* (__fastcall* ReadResType)(const char* fileName, DWORD* bytesRead, BOOL isFile);


public:
	ResHackFastcall(uintptr_t thReadRes, uintptr_t thMalloc);

protected:
	static void* __fastcall MyReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile);
	virtual void* CallOriginalReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile) override;
};

class ResHackStdcall final : public ResHackBase
{
protected:
	typedef void* (__stdcall* ReadResType)(const char* fileName, DWORD* bytesRead, BOOL isFile);


public:
	ResHackStdcall(uintptr_t thReadRes, uintptr_t thMalloc);

protected:
	static void* __stdcall MyReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile);
	virtual void* CallOriginalReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile) override;
};

class ResHackFirstArgAtEax final : public ResHackBase
{
public:
	ResHackFirstArgAtEax(uintptr_t thReadRes, uintptr_t thMalloc);

protected:
	static void* MyReadResWrapper(DWORD* bytesRead, BOOL isFile);
	static void* __stdcall MyReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile);
	virtual void* CallOriginalReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile) override;
};

class ResHackTH07 final : public ResHackBase
{
protected:
	typedef void* (__fastcall* ReadResType)(const char *fileName, BOOL isFile);

	DWORD* const m_bytesRead = NULL;


public:
	ResHackTH07(uintptr_t thReadRes, uintptr_t thMalloc, uintptr_t bytesRead);

protected:
	static void* __fastcall MyReadRes(const char *fileName, BOOL isFile);
	virtual void* CallOriginalReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile) override;
};

class ResHackTH06 final : public ResHackBase
{
protected:
	typedef void* (__cdecl* ReadResType)(const char *fileName, BOOL isFile);

	DWORD* const m_bytesRead = NULL;


public:
	ResHackTH06(uintptr_t thReadRes, uintptr_t thMalloc, uintptr_t bytesRead);

protected:
	static void* __cdecl MyReadRes(const char *fileName, BOOL isFile);
	virtual void* CallOriginalReadRes(const char* fileName, DWORD* bytesRead, BOOL isFile) override;
};
