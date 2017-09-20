#pragma once


class InlineHook final
{
private:
	void* const m_originalFunction;
	void* const m_hookFunction;

	BYTE m_originalCode[5];

	bool m_isEnabled = false;


public:
	InlineHook(void* originalFunction, void* hookFunction, bool enable = true);
	~InlineHook();

	bool Enable();
	bool Disable();

	bool IsEnabled()
	{
		return m_isEnabled;
	}

	template<class T = void*>
	T GetOriginalFunction()
	{
		return (T)m_originalFunction;
	}
};
