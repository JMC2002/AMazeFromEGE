#include "Screen.h"

namespace screen {
	Screen::Screen()
	{
		ege::initgraph(Width, Height,ege::INIT_ANIMATION);	//��ʼ��ͼ�ν���
		ege::setbkcolor(ege::WHITE);	//���ñ�����ɫΪ��ɫ
		ege::ege_enable_aa(true);
		ege::setrendermode(ege::RENDER_MANUAL);
		ege::setcaption("SCRABBLE!!!!!");
	}

	Screen::~Screen()
	{
		ege::closegraph();	//�ر�ͼ�ν���
	}
}