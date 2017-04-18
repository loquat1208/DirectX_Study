#include "systemclass.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow ) {
	SystemClass* system;
	bool result;

	//system객체 생성
	system = new SystemClass;
	if ( !system ) {
		return 0;
	}

	//초기화하고 실행
	result = system->Initialize( );
	if ( result ) {
		system->Run( );
	}

	//system을 종료하고 메모리 정리
	system->Shutdown( );
	delete system;
	system = 0;

	return 0;
}


