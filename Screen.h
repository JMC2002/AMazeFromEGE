/*****************************************************************//**
 * \file   Screen.h
 * \brief  ά����ʾ���ڵ�һЩ����
 * 
 * \date   April 2022
 *********************************************************************/
#pragma once
#define SHOW_CONSOLE
#include <ege.h>

namespace screen {
	// ��Ļ��С
	enum ScreenParam
	{
		Width = 1940,
		Height = 1080
	};

	class Screen
	{
		Screen();
		~Screen();
	public:
		// Meyers' Singleton, C++11���Ժ��̰߳�ȫ�ĵ���ģʽ
		static Screen& getScreen()
		{
			static Screen screen;
			return screen;
		}

		// ��ֹ����
		Screen(const Screen& oth) = delete;
		Screen& operator=(const Screen& oth) = delete;
	};
}