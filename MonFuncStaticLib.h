//블루트스연결 제어의 헤더 

#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
#include <BluetoothAPIs.h>

extern int state;
extern BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio;
extern BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params;
extern BLUETOOTH_DEVICE_INFO m_device_info;
extern BLUETOOTH_DEVICE_INFO c_device_info;
extern BLUETOOTH_RADIO_INFO m_bt_info;

namespace Monitoring
{
	class MonitoringFuncs
	{
	public:
		static void InitMon(DWORD);                                                      //쓰레드 받는 함수
		static void startMon();                                                          //모니터링 시작함수
		static bool stopMon();                                                           //모니터링 중지 함수
		static void UnInitMon();                                                         //쓰레드 닫는 함수
		static void checkConn(BLUETOOTH_DEVICE_INFO &, BLUETOOTH_DEVICE_INFO &);         //연결 체크 함수
		static void bthMain();                                                           //메인 진입 함수
	};
}

//전체적인 흐름 :: 메인 앱에서 쓰레드를 생성하고 InitMon(DWORD)를 호출해서 쓰레드 ID를 전달한다 > InitMon(DWORD)안에서 startMon()를 호출한다
//> startMon()안에서 ThreadProc_BT가 호출된다. > ThreadProc_BT가 bthMain()를 호출해 메인으로 들어간다. > checkConn가 실행되고 모니터링을 한다
// 종료는 메인 앱쓰레드에서 UnInitMon()을 호출하면 쓰레드가 닫히게 된다.
