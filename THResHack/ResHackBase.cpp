#include "stdafx.h"
#include "ResHackBase.h"
#include <string>
#include <fstream>
using namespace std;


// --------------------------------------------------------------------------------------
// ResHackBase
// --------------------------------------------------------------------------------------

ResHackBase::ResHackBase(void* thReadRes, void* myReadRes, MallocType thMalloc) :
	m_thMalloc(thMalloc),
	m_readResHook(thReadRes, myReadRes)
{
}

void* ResHackBase::MyReadRes(const char* _fileName, DWORD* bytesRead, BOOL isFile)
{
	string fileName = _fileName;
	size_t pos = fileName.rfind('\\');
	if (pos != string::npos)
		fileName = fileName.substr(pos + 1);
	pos = fileName.rfind('/');
	if (pos != string::npos)
		fileName = fileName.substr(pos + 1);

	ifstream f("data\\" + fileName, ifstream::binary);
	if (!f.is_open())
	{
		m_readResHook.Disable();
		void* buffer = CallOriginalReadRes(_fileName, bytesRead, isFile);
		m_readResHook.Enable();
		return buffer;
	}

	f.seekg(0, ifstream::end);
	size_t size = (size_t)f.tellg();
	if (bytesRead != NULL)
		*bytesRead = size;
	void* buffer = m_thMalloc(size);
	f.seekg(0);
	f.read((char*)buffer, size);
	return buffer;
}


// --------------------------------------------------------------------------------------
// ResHackFactory
// --------------------------------------------------------------------------------------

ResHackBase& ResHackFactory::GetResHackInstance()
{
	if (!m_resHackInstance)
	{
		for (const auto& i : m_generators)
		{
			if (memcmp(i.signatureAddr, &i.signature->front(), i.signature->size() * sizeof(BYTE)) == 0)
			{
				m_resHackInstance = i.generator();
				break;
			}
		}
		if (!m_resHackInstance)
			MessageBox(NULL, _T("Unknown version of game"), _T("THResHack"), MB_ICONERROR);
	}

	return *m_resHackInstance;
}
