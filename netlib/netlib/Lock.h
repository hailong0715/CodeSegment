/*================================================================
*   Copyright (C) 2014 All rights reserved.
*
*   Êñá‰ª∂ÂêçÁß∞ÔºöLock.h
*   Âà?Âª?ËÄÖÔºöZhang Yuanhao
*   ÈÇ?   ÁÆ±Ôºöbluefoxah@gmail.com
*   ÂàõÂª∫Êó•ÊúüÔº?014Âπ?9Êú?0Êó?
*   Êè?   Ëø∞Ôºö
*
#pragma once
================================================================*/

#ifndef __LOCK_H__
#define __LOCK_H__

#include "ostype.h"

class CLock
{
public:
	CLock();
	virtual ~CLock();
	void lock();
	void unlock();
#ifndef _WIN32
	pthread_mutex_t& getMutex() { return m_lock; }
	virtual bool try_lock();
#endif
private:
#ifdef _WIN32
	CRITICAL_SECTION m_critical_section;
#else
	pthread_mutex_t m_lock;
#endif
};

#ifndef _WIN32
class CRWLock
{
public:
	CRWLock();
	virtual ~CRWLock();
	void rlock();
	void wlock();
	void unlock();
	bool try_rlock();
	bool try_wlock();
private:
	pthread_rwlock_t m_lock;
};

class CAutoRWLock
{
public:
	CAutoRWLock(CRWLock* pLock, bool bRLock = true);
	virtual ~CAutoRWLock();
private:
	CRWLock* m_pLock;
};

#endif

class CAutoLock
{
public:
	CAutoLock(CLock* pLock);
	virtual ~CAutoLock();
private:
	CLock* m_pLock;
};

#endif
