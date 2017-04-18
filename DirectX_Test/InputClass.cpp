#include "inputClass.h"

InputClass::InputClass( ) {
}

InputClass::InputClass( const InputClass& other ) {
}

InputClass::~InputClass( ) {
}

void InputClass::Initialize( ) {
	int i;

	//모든 키를 초기화
	for ( i = 0; i < 256; i++ ) {
		m_key[ i ] = false;
	}

	return;
}

void InputClass::KeyDown( unsigned int input ) {
	m_key[ input ] = true;
	return;
}

void InputClass::KeyUp( unsigned int input ) {
	m_key[ input ] = false;
	return;
}

bool InputClass::IsKeyDown( unsigned int key ) {
	return m_key[ key ];
}
