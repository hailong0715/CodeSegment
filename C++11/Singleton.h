#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename	T>
class Singleton
{
public:
	template <typename... Args>
	static T* Intstance(Args&& ... args)
	{
		if (nullptr == m_pIntsnace)
			m_pIntsnace = new T(std::forward<Args>(args)...);
		return m_pIntsnace;
	}
	static T* GetInstace()
	{
		if (nullptr == m_pIntsnace)
			throw std::logic_error("the instance is not init");
		return m_pIntsnace;
	}

private:
	static T* m_pIntsnace;
	Singleton(void);
	~Singleton(void)=default;
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
};

template<typename T> T* Singleton<T>::m_pIntsnace = nullptr;
#endif