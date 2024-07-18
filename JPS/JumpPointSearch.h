#pragma once
#include <array>
#include <queue>
#include <unordered_map>



enum Direction
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	D_UP_RIGHT,
	D_UP_LEFT,
	D_DOWN_RIGHT,
	D_DOWN_LEFT,
	DIRECTION_COUNT
};


struct Point 
{
	struct Hash
	{
		std::size_t operator()(const Point& p) const {
			std::size_t h1 = std::hash<int>()(p._xpos);
			std::size_t h2 = std::hash<int>()(p._ypos);
			return h1 ^ (h2 << 1);
		}
	};

	int _xpos;
	int _ypos;
	bool operator==(const Point& other) const { return _xpos == other._xpos && _ypos == other._ypos; }
	bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Node {

	struct compare
	{
		bool operator()(const Node* v1, const Node* v2) const
		{
			if (v1->_F == v2->_F)
				return v1->_H < v2->_H;
			return v1->_F > v2->_F;
		}
	};

	Point _point;
	Node* _parent;
	double _G;
	double _H;
	double _F;
	Direction _reason;
	Direction _fromParent;

	bool operator==(const Node& v) const { return this->_point == v._point; }
	bool operator!=(const Node& v) const { return !(this->_point == v._point); }

};

struct DIRECTIONS
{
	int _xpos;
	int _ypos;
	Direction direction;
};
constexpr std::array<DIRECTIONS, 8> directions{ {
		//       X    Y      Direction
				{0  , -1    , UP},
				{0  , 1     , DOWN},
				{1  , 0     , RIGHT},
				{-1 , 0     , LEFT},
				{1  , -1    , D_UP_RIGHT},
				{-1 , -1    , D_UP_LEFT},
				{1  , 1     , D_DOWN_RIGHT},
				{-1 , 1     , D_DOWN_LEFT}
} };

class JumpPointSearch
{
public:
	void search();
	void clearPath(bool isClearAll);
	bool isFind() const { return _find ? true : false; }
	Node* getFind() const { return _find; }
	void init(Point start, Point end);

	JumpPointSearch() = default;
	JumpPointSearch(Point start, Point end);
	~JumpPointSearch();
private:

	void checkUP(Node* parent, int col, int row);
	void checkDOWN(Node* parent, int col, int row);
	void checkRIGHT(Node* parent, int col, int row);
	void checkLEFT(Node* parent, int col, int row);

	void checkD_UP_RIGHT(Node* parent, int col, int row);
	void checkD_DOWN_RIGHT(Node* parent, int col, int row);
	void checkD_UP_LEFT(Node* parent, int col, int row);
	void checkD_DOWN_LEFT(Node* parent, int col, int row);

	void openListPush(Node* parent, int col, int row, Direction reason, Direction fromParent);
	void findEnd(Node* parent, int col, int row);
	Node* createNode(Node* parent, Point newPoint, Direction reason = NONE, Direction fromParent = NONE);
	bool check(Node* parent, int col, int row, Direction fromParent);

	double getH(Point start, Point end) const;
	double getG(Point start, Point end) const;
	
private:
	Node* _find{ nullptr };
	Point _start{ -1, -1 };
	Point _end{ -1,-1 };
	std::priority_queue<Node*, std::vector<Node*>, Node::compare> _openList;
	std::unordered_map<Point, Node*, Point::Hash> _openListMap;
	std::vector<Node*> _closeList;
};


