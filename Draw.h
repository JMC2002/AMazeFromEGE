#pragma once
/*****************************************************************//**
 * \file   Draw.h
 * \brief  һЩ�����Ļ��Ʋ���
 * 
 * \date   April 2022
 *********************************************************************/
#define SHOW_CONSOLE
#include <initializer_list>
#include <cmath>
#include <tuple>
#include <ege.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>
#include <functional>
#include "Maze.h"
namespace basic {
	struct Point
	{
		float x{}, y{};
		Point(int x, int y)
			:x(x), y(y) {}
		Point(float x, float y)
			:x(x), y(y) {}
	};

	class Text
	{
		std::string text;	// �ı�
		Point p;			// ����λ��
		int h;				// ����߶�
		int w{};			// �����ȣ�0Ϊ����Ӧ
		ege::color_t color;	// ������ɫ

	public:
		Text() = default;
		Text(const Point& p, std::string t, int h = 10, int w = 0, ege::color_t color = ege::BLACK)
			:p(p), text(t), h(h), w(w), color(color) {}
		Text(int x, int y, std::string t, int h = 10, int w = 0, ege::color_t color = ege::BLACK)
			:Text({ x,y }, t, h, w, color) {}

		void show()const;
	};

	class Button
	{
	public:
		using OP = std::function<void(void)>;
		Button(int x, int y, int w, int h, std::string t, const OP& op, ege::color_t color = EGEARGB(0xFF, 0x1E, 0x90, 0xFF))
			:x(x), y(y), width(w), height(h), text({ x + w / 2,y + h / 2 }, t, h / 3), operate(op), color(color) {}
		Button(const Point& p, int w, int h, std::string t, const OP& op, ege::color_t color = EGEARGB(0xFF, 0x1E, 0x90, 0xFF))
			:Button(static_cast<int>(p.x), static_cast<int>(p.y), w, h, t, op, color) {}

		virtual void draw()const = 0;
		virtual bool isInButton(const Point& p)const = 0;
		virtual bool isInButton(int x, int y)const { return isInButton({ x,y }); }
		virtual bool isClick() = 0;
		virtual void run()const { operate(); }
	protected:
		int x, y;			// ���Ͻ�
		int width, height;	// ��������
		Text text;			// ��ʾ�ı�
		ege::color_t color;	// �����ɫ
		std::function<void(void)> operate; // ����ע����Ϊ
	};

	class RoundRectButton
		:public Button
	{
	public:
		RoundRectButton(const Point& p, int w, int h, float r, std::string t, const OP& op, ege::color_t color = EGEARGB(0xFF, 0x1E, 0x90, 0xFF))
			:Button(p, w, h, t, op, color), radius(r) {}
		RoundRectButton(int x, int y, int w, int h, float r, std::string t, const OP& op, ege::color_t color = EGEARGB(0xFF, 0x1E, 0x90, 0xFF))
			:Button(x, y, w, h, t, op, color), radius(r) {}

		virtual void draw()const override;
		virtual bool isInButton(const Point& p)const override;
		virtual bool isInButton(int x, int y)const { return isInButton({ x,y }); }
		virtual bool isClick() override;
	private:
		float radius;
		std::function<void(void)> operate; // ����ע����Ϊ
	};

	// ������ (��)
	class RoundRectButtons
	{
		Point p;			// ���Ͻ�
		int width, height;	// ����������Ŀ��
		float radius;		// ÿ������Բ���İ뾶
		int dis;			// ÿ������Բ���ļ��

		std::vector<RoundRectButton> buttons;
	public:
		RoundRectButtons(const Point& p, int w, int h, float r, int d, const std::vector<std::string>& texts, std::vector<RoundRectButton::OP> ops)
			:p(p), width(w), height(h), radius(r), dis(d)
		{
			int tmpH = (h - d * (texts.size() - 1)) / texts.size();
			int tmpD = tmpH + d;
			int tmpY = p.y - tmpD;
			std::transform(texts.cbegin(), texts.cend(), ops.cbegin(), std::back_inserter(buttons), [&](auto& i, auto& j) {return RoundRectButton(p.x, tmpY += tmpD, w, tmpH, r, i, j); });
		}
		RoundRectButtons(int x, int y, int w, int h, float r, int d, const std::vector<std::string>& texts, std::vector<RoundRectButton::OP> ops)
			:RoundRectButtons({ x,y }, w, h, r, d, texts, ops) {}

		void show()const;
		bool run()const;
	};

	class Picture
	{
		ege::PIMAGE pimg{};
	public:
		Picture(const std::string& src, int w, int h)
			:pimg(ege::newimage(w,h))
		{
			using namespace ege;
			PIMAGE tmp = newimage();
			getimage(tmp, src.c_str());
			int tmpW = getwidth(tmp);
			int tmpH = getheight(tmp);
			putimage(pimg, 0, 0, w, h, tmp, 0, 0, tmpW, tmpH);
			delimage(tmp);
			tmp = nullptr;
		}

		Picture(const Picture& oth)
		{
			int tmpW = getwidth(oth.pimg);
			int tmpH = getheight(oth.pimg);
			pimg = ege::newimage(tmpW, tmpH);
			ege::getimage(pimg, oth.pimg, 0, 0, tmpW, tmpH);
		}

		Picture(const ege::PIMAGE& p) :pimg(p) {}

		~Picture() {
			ege::delimage(pimg);
			pimg = nullptr;
		}

		// �����Ͻ� (x, y) ��չʾͼƬ
		void show(int x, int y) {
			ege::putimage(x, y, pimg);
		}
	};

	class Block
	{
		int w, h;
		//std::vector<ege::PIMAGE> blocks;
		std::vector<Picture> blocks;
	public:
		enum block {
			begin = -1,
			wall,
			road,
			way,
			end
		};
		static ege::color_t Color[3];

		Block(int w, int h);

		~Block() {
			for (auto& i : blocks)
			{
				//ege::delimage(i);
			}
		}

		int getW()const { return w; }
		int getH()const { return h; }

		void draw(const Point& p, block num);

		void drawWall(const Point& p);	// �����Ͻ� p ����ǽ
		void drawRoad(const Point& p);  // �����Ͻ� p �����հ�·
		void drawWay(const Point& p);	// �����Ͻ� p ����·��

		// ��ֹ����
		Block(const Block& oth) = delete;
		Block& operator=(const Block& oth) = delete;
	};

	class Draw
	{
		Point p;		// �������Ͻ�
		maze::Solute m;
		Block b;
		int row, cal;
	public:
		Draw(const Point& p, int a, int b, int w, int h)
			:p(p), m(a, b), b(w / a, h / b), row(a), cal(b) {
			show();
		}

		void show();
		void solve() { m.solve(); show(); }
		void remake() { m = maze::Solute(row, cal); show(); }
	};
}