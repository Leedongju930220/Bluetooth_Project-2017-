//이 코드는 다른 코드에서 보낸 메세지를 잘 받고 있는지를 확인하는 코드입니다.

#include <stdio.h>
#include "사용할헤더.h"

#define     THRD_MESSAHE_CONNECT_DEVICE   WM_APP	//첫 기기연결
#define		THRD_MESSAGE_SOMEWORK		  WM_APP + 1 //스크린락	
#define		THRD_MESSAGE_EXIT			  WM_APP + 2 //스크린락 해지
#define     THRD_MESSAGE_NOSIGNAL_DEVICE  WM_APP + 3 //블루투스 꺼져 있음
#define     THRD_MESSAGE_NODEVICE         WM_APP + 4 //연결된 기기 없음
#define     THRD_CLOSE					  WM_APP + 5  //모든쓰레드종료

//쓰레드간 통신으로 메세지를 사용하였고 각 메세지에 해당하는 WM_APP을 지정하였다

using namespace Monitoring;

void M_start();
void M2_start();

DWORD WINAPI ThreadProc_MAPP1(LPVOID);
DWORD WINAPI ThreadProc_MAPP2(LPVOID);

DWORD MAPP_Thread_ID1 = 0;
DWORD MAPP_Thread_ID2 = 0;

int MSG_EXIT = 0;

int main()
{
	HANDLE MAPP_Thread1;
	HANDLE MAPP_Thread2;

	char *APP_Thread_Msg1 = "(text.cpp)   블루투스 체크 쓰레드 생성완료 "; // 프록시파라미터
	char *APP_Thread_Msg2 = "(text.cpp)   블루투스 체크 쓰레드 생성완료2 "; // 프록시파라미터

	MAPP_Thread1 = CreateThread(NULL, 0, ThreadProc_MAPP1, (LPVOID)APP_Thread_Msg1, 0, &MAPP_Thread_ID1);
	MAPP_Thread2 = CreateThread(NULL, 0, ThreadProc_MAPP2, (LPVOID)APP_Thread_Msg2, 0, &MAPP_Thread_ID2);

	WaitForSingleObject(MAPP_Thread1, INFINITE);
	WaitForSingleObject(MAPP_Thread2, INFINITE);

	printf("(text.cpp)   시작2\n");
	

	return 0;
}


void MSGfunc()
{

	MSG msg = { 0 };
	printf("메인체크함수 메세지접근\n");
	while (!MSG_EXIT){
		printf("에러1 %d\n", GetLastError());                          //GetLastError()함수는 프로그램에서 무슨 오류가 생겼는지 확인할 수 있는 함수이다
                                                                  // 번호가 출력되는데 구글에 검색해보면 해당 번호가 무슨 에러인지 알 수 있다.
		printf("(check_message.cpp)   메인체크함수 메세지접근 후 whlie접근\n");
		Sleep(1000);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			printf("에러2 :::%d\n", GetLastError());
			printf("(check_message.cpp)   메세지도는중\n");
			if (msg.message == WM_APP + 1)
			{
				printf("(check_message.cpp)   스크린락\n");
			}
			if (msg.message == WM_APP +2)
			{
				printf("(check_message.cpp)   스크린락해제\n");
			}
			if (msg.message == WM_APP + 3)
			{
				printf("(check_message.cpp)   블루투스 꺼져있음\n");
			}
			if (msg.message == WM_APP + 4)
			{
				printf("(check_message.cpp)   연결된 기기 없음\n");
			}
			if (msg.message == WM_APP + 5)
			{
				printf("(check_message.cpp)   모든 쓰레드 종료\n");
				PostThreadMessage(MAPP_Thread_ID, WM_APP + 5, 0, 0);
			}

			DispatchMessage(&msg);
			printf("에러3%d\n", GetLastError());
		}
		printf("(check_message.cpp)MSG쓰레드 마지막부분\n");
		
	}
}

DWORD WINAPI ThreadProc_MAPP1(LPVOID M_param)           //이 함수는 블루투스를 연결하고 체크하는 함수를 호출하는 쓰레드이다
{
	char *TP_Msg = (char *)M_param; // 프록시로 들어온 파라미터

	printf("(check_message.cpp)   %s :: 메세지 쓰레드 ID: %d\n", TP_Msg, MAPP_Thread_ID1);
	      
	Monitoring::MonitoringFuncs::startMon(MAPP_Thread_ID1);
	printf("(check_message.cpp)   블루투스제어함수 호출함수 스레드 나가기전\n");
	return (DWORD)M_param;
}


DWORD WINAPI ThreadProc_MAPP2(LPVOID M_param)           //이 함수는 블루투스를 제어하는 코드가 보낸 메세지를 확인하는 쓰레드이다.
{
	char *TP_Msg = (char *)M_param; // 프록시로 들어온 파라미터
  
	MSGfunc();
	printf("(check_message.cpp)  %s :: 메세지 쓰레드 ID: %d\n", TP_Msg, MAPP_Thread_ID2);
		
	printf("(check_message.cpp)  메세지체크함수 스레드 나가기전\n");
	return (DWORD)M_param;
}













