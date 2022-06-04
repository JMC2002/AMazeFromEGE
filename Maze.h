#pragma once
/*****************************************************************//**
 * \file   Maze.h
 * \brief  �Թ������࣬ͬʱ�ṩ��ӡ����׼������Ľӿ�
 * 
 * \date   April 2022
 *********************************************************************/
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <string>
#include <stack>
#include <functional>
#include <algorithm>
//#include <cassrt>
namespace maze {
	using blockpic = std::vector<std::string>;
	const blockpic block{
			"��","  ","��"
	};
	class locat {
	public:
		using rowType = size_t;
		using calType = size_t;

		locat(rowType r = 0, calType c = 0)
			:loc(r, c) {}

		rowType x(void)const { return loc.first; }	//����һά����	
		rowType x(const rowType& r) { loc.first = r; return loc.first; }//�޸Ĳ�����һά����		
		calType y(void)const { return loc.second; }		//���ض�ά����
		calType y(const calType& c) { loc.second = c; return loc.second; }//�޸Ĳ����ض�ά����
		locat up(void)const { return { loc.first - 1, loc.second }; }
		locat down(void)const { return { loc.first + 1, loc.second }; }
		locat left(void)const { return { loc.first, loc.second - 1 }; }
		locat right(void)const { return { loc.first, loc.second + 1 }; }
		locat& operator()(const rowType& r, const calType& c) {
			x(r), y(c);
			return *this;
		}
		locat& operator()(const locat& oth) {
			return (*this)(oth.x(), oth.y());
		}
		bool operator<(const locat& oth)const {
			return x() == oth.x() ? y() < oth.y() : x() < oth.x();
		}
		bool operator==(const locat& oth)const {
			return x() == oth.x() && y() == oth.y();
		}
		friend std::ostream& operator<<(std::ostream& o, const locat& l)
		{
			o << '(' << l.x() << ',' << l.y() << ')';
			return o;
		}
	private:
		std::pair<rowType, calType> loc;
	};

	class Maze
	{
	public:
		using rowType = locat::rowType;
		using calType = locat::calType;
		using locats = std::vector<locat>;

		enum blockType {
			wall,
			road,
			way
		};

		Maze(const locat& l) :xyMsg(l), Map(l.x(), mazeLine(l.y(), blockType::wall)) {}
		Maze(rowType row, calType cal);		// �������һ���Թ�������Prim�㷨

		std::function<locat(const locat& l)> operat[4]{
			[](const locat& l) {return l.up(); },
			[](const locat& l) {return l.down(); },
			[](const locat& l) {return l.left(); },
			[](const locat& l) {return l.right(); },
		};

		auto& operator()(const rowType& r, const calType& c) {
			return Map[r][c];
		}
		auto& operator()(const locat& p) {
			return (*this)(p.x(), p.y());
		}

		rowType row(void) { return xyMsg.x(); }	// �����Թ�������
		calType cal(void) { return xyMsg.y(); }	// �����Թ�������
		locat& start(void) { return _start; }
		locat& end(void) { return _end; }
		void show(const blockpic pic = block);	// ��ӡ�Թ�

	private:
		using mazeLine = std::vector<blockType>;		// �����Թ�
		using mazeMap = std::vector<mazeLine>;			// �Թ�ͼ

		locats findWall(const locat& p);	//����һ��·��Χ��ǽ
		locats findRoad(const locat& p);	//����һ��ǽ��Χ��·

		locat xyMsg;
		mazeMap Map;
		locat _start, _end;
	};

	//�����Թ�����Ľ������
	class Solute
		:public Maze
	{
	public:
		Solute(const rowType& r, const calType& c)
			:Maze(r, c) {
		}

		bool isIn(const locat& p) {
			return p.x() < row() && p.y() < cal();
		}
		void solve();
		bool solve(const locat& p);
		void showWay(void) {
			if (!ans.empty()) {
				std::cout << ans.top();
				ans.pop();
				if (!ans.empty())
					std::cout << " -> ";
				showWay();
			}
		};
	private:
		Maze mark{ locat{row(),cal()} };
		std::stack<locat> ans{};
	};
}