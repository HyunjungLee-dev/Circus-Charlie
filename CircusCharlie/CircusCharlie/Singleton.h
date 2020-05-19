#pragma once

template <typename T>
class Singleton
{
private:
	static T* _Singleton;
public:
	Singleton() { }
	virtual ~Singleton() { }

	static T* GetSingleton()
	{
		if (_Singleton == NULL)
		{
			_Singleton = new T;
		}
		return _Singleton;
	}

	static void Release()
	{
		delete _Singleton;
		_Singleton = NULL;
	}
};

//template<typename T> T* Singleton<T> ::_Singleton = NULL;
