/*
* Copyright (c) 2013-2022, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#ifndef WIN32SERVICE_H__
#define WIN32SERVICE_H__

#include <functional>
#include <thread>
#include <windows.h>

#define SERVICE_NAME "i2pdService"

class I2PService
{
	public:

		I2PService(PSTR pszServiceName,
			BOOL fCanStop = TRUE,
			BOOL fCanShutdown = TRUE,
			BOOL fCanPauseContinue = FALSE);

		virtual ~I2PService(void);

		static BOOL isService();
		static BOOL Run(I2PService &service);
		void Stop();

		typedef std::function<bool ()> DaemonStart;
		void SetDaemonStart (const DaemonStart& f) { m_daemonStart = f; };

		typedef std::function<bool ()> DaemonStop;
		void SetDaemonStop (const DaemonStop& f) { m_daemonStop = f; };


	protected:

		virtual void OnStart(DWORD dwArgc, PSTR *pszArgv);
		virtual void OnStop();
		virtual void OnPause();
		virtual void OnContinue();
		virtual void OnShutdown();
		void SetServiceStatus(DWORD dwCurrentState,
			DWORD dwWin32ExitCode = NO_ERROR,
			DWORD dwWaitHint = 0);

	private:

		static void WINAPI ServiceMain(DWORD dwArgc, LPSTR *lpszArgv);
		static void WINAPI ServiceCtrlHandler(DWORD dwCtrl);
		void WorkerThread();
		void Start(DWORD dwArgc, PSTR *pszArgv);
		void Pause();
		void Continue();
		void Shutdown();
		static I2PService* s_service;
		PSTR m_name;
		SERVICE_STATUS m_status;
		SERVICE_STATUS_HANDLE m_statusHandle;

		BOOL m_fStopping;
		HANDLE m_hStoppedEvent;

		std::thread* _worker;

	private:

		DaemonStart m_daemonStart;
		DaemonStop m_daemonStop;
};

#endif // WIN32SERVICE_H__
