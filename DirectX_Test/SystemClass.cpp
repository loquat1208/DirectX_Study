#include "systemclass.h"

SystemClass::SystemClass( ) {
	m_input = 0;
	m_graphics = 0;
}

SystemClass::SystemClass( const SystemClass& other ) {
}

SystemClass::~SystemClass( ) {
}

bool SystemClass::Initialize( ) {
	int screen_width = 0;
	int screen_height = 0;
	bool result = false;

	InitializeWindows( screen_width, screen_height );

	//input객체 생성. 키보드 입력 처리
	m_input = new InputClass;
	if ( !m_input ) {
		return false;
	}
	
	m_input->Initialize( );

	//graphics객체 생성. 그래픽 처리
	m_graphics = new GraphicsClass;
	if ( !m_graphics ) {
		return false;
	}

	result = m_graphics->Initialize( screen_width, screen_height, m_hwnd );
	if ( !result ) {
		return false;
	}

	return true;
}

void SystemClass::Shutdown( ) {
	if ( m_graphics ) {
		m_graphics->Shutdown( );
		delete m_graphics;
		m_graphics = 0;
	}

	if ( m_input ) {
		delete m_input;
		m_input = 0;
	}

	ShutdownWindows( );

	return;
}

void SystemClass::Run( ) {
	MSG msg;
	bool done;
	bool result;

	//메세지 구조체 초기화
	ZeroMemory( &msg, sizeof( MSG ) );

	//종료버튼을 누르기 전까지 무한무프
	done = false;
	while ( !done ) {
		//윈도우 메세지 처리
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( msg.message == WM_QUIT ) {
			done = true;
		} else {
			result = Frame( );
			if ( !result ) {
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame( ) {
	bool result;

	if ( m_input->IsKeyDown( VK_ESCAPE ) ) {
		return false;
	}

	result = m_graphics->Frame( );
	if ( !result ) {
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) {
	switch ( umsg ) {
	case WM_KEYDOWN:
		m_input->KeyDown( ( unsigned int )wparam );
		return 0;
	case WM_KEYUP:
		m_input->KeyUp( ( unsigned int )wparam );
		return 0;
	default:
		return DefWindowProc( hwnd, umsg, wparam, lparam );
	}
}

void SystemClass::InitializeWindows( int& screenWidth, int& screenHeight ) {
	WNDCLASSEX wc;
	DEVMODE dm_screen_settings; 
	int pos_x = 0; 
	int	pos_y = 0;

	//외부 포인터를 이 객체로 설정
	ApplicationHandle = this;

	//이 어플리케이션의 인스턴스를 가져온다.
	m_hinstance = GetModuleHandle( NULL );

	//어플리케이션의 이름
	m_application_name = L"DirectX Test";

	//윈도우 클래스를 기본 설정으로
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_application_name;
	wc.cbSize = sizeof( WNDCLASSEX );

	//윈도우 클래스를 등록
	RegisterClassEx( &wc );

	//모니터 해상도
	screenWidth = GetSystemMetrics( SM_CXSCREEN );
	screenHeight = GetSystemMetrics( SM_CYSCREEN );

	//풀스크린 모드 변수의 값에 따라 화면 설정
	if ( FULL_SCREEN ) {
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 합니다. 
		memset( &dm_screen_settings, 0, sizeof( dm_screen_settings ) );
		dm_screen_settings.dmSize = sizeof( dm_screen_settings ); 
		dm_screen_settings.dmPelsWidth = ( unsigned long )screenWidth; 
		dm_screen_settings.dmPelsHeight = ( unsigned long )screenHeight; 
		dm_screen_settings.dmBitsPerPel = 32;	
		dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; 
	}
	else {
		screenWidth = 800;
		screenHeight = 600;

		pos_x = ( GetSystemMetrics( SM_CXSCREEN ) - screenWidth ) / 2;
		pos_y = ( GetSystemMetrics( SM_CYSCREEN ) - screenHeight ) / 2;
	}

	//설정한 것을 창으로 만들고 핸들을 가져온다.
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW, m_application_name, m_application_name, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		pos_x, pos_y, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//윈도우를 화면에 표시하고 포커스를 줌
	ShowWindow( m_hwnd, SW_SHOW );
	SetForegroundWindow( m_hwnd );
	SetFocus( m_hwnd );

	//마우스 커서를 지움
	ShowCursor( false );

	return;
}

void SystemClass::ShutdownWindows( ) {
	// 마우스 커서를 표시합니다. 
	ShowCursor( true ); 

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿉니다. 
	if( FULL_SCREEN ) { 
		ChangeDisplaySettings( NULL, 0 ); 
	} 

	// 창을 제거합니다. 
	DestroyWindow( m_hwnd ); 
	m_hwnd = NULL; 

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass( m_application_name, m_hinstance);
	m_hinstance = NULL; 
	
	// 이 클래스에 대한 외부 포인터 참조를 제거합니다. 
	ApplicationHandle = NULL; 
	
	return;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam ) { 
	switch( umessage ) { 
		// 윈도우가 제거되었는지 확인합니다. 
	case WM_DESTROY: 
		PostQuitMessage( 0 ); 
		return 0; 
		// 윈도우가 닫히는지 확인합니다. 
	case WM_CLOSE: 
		PostQuitMessage( 0 );	
		return 0;
		// 다른 모든 메세지들은 system 클래스의 메세지 처리기에 전달합니다. 
	default:  
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam); 
	}
}
