#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
#include <string>
#include <cmath>
#include <cassert>

// TODO: point and rectangle class definitions
struct point {
	//given ints
	point(int x, int y) :x(x), y(y) {}
	//copy constructor
	point(point const& p1) :x(p1.x), y(p1.y) {}
	//default
	point() {}
	//member fun overrides
	point& operator-=(point const& p1) {
		x -= p1.x;
		y -= p1.y;
		return *this;
	}
	point& operator+=(point const& p1) {
		x += p1.x;
		y += p1.y;
		return *this;
	}
	point& operator*=(point const& p1) {
		x *= p1.x;
		y *= p1.y;
		return *this;
	}
	point& operator/=(point const& p1) {
		x /= p1.x;
		y /= p1.y;
		return *this;
	}

	int x = 0;
	int y = 0;
};

//(point add/sub within rectangle demands this be here)
//subtraction
point operator-(point p1, point p2) {
	return point(p1.x - p2.x, p1.y - p2.y);
}
//addition 
point operator+(point p1, point p2) {
	return point(p1.x + p2.x, p1.y + p2.y);
}
struct rectangle {
	//constructors
	//given points
	rectangle(point p1, point p2) :top_left(p1), bottom_right(p2) {}
	//give a single point
	rectangle(point p1) :top_left(p1), bottom_right(p1) {}
	//given ints for points
	rectangle(int w, int x, int y, int z) : top_left(w, x), bottom_right(y, z) {}
	//given a rectangle (copy constructor)
	rectangle(rectangle const& r1) :top_left(r1.top_left), bottom_right(r1.bottom_right) {}
	//default
	rectangle() {}
	//member functions
	rectangle& operator+=(point const& p1) {
		top_left += p1;
		bottom_right += p1;
		return *this;
	}
	rectangle& operator-=(point const& p1) {
		top_left -= p1;
		bottom_right -= p1;
		return *this;
	}
	rectangle& operator*=(point const& p1) {
		top_left *= p1;
		bottom_right *= p1;
		return *this;
	}
	rectangle& operator/=(point const& p1) {
		top_left /= p1;
		bottom_right /= p1;
		return *this;
	}
	point top_right() {
		return point(bottom_right.x, top_left.y);
	}
	point bottom_left() {
		return point(top_left.x, bottom_right.y);
	}
	int top() {
		return top_left.y;
	}
	int left() {
		return top_left.x;
	}
	int right() {
		return bottom_right.x;
	}
	int bottom() {
		return bottom_right.y;
	}
	int width() {
		return bottom_right.x - top_left.x;
	}
	int height() {
		//return top_left.y - bottom_right.y;
		return bottom_right.y - top_left.y;
	}
	point extent() {
		//returns size of rectangle (width, height)
		return point(bottom_right.x - top_left.x, bottom_right.y - top_left.y);
	}
	void extent(point p1) {
		//resize rectangle
		bottom_right = top_left + p1;
	}
	rectangle& inflate(point p1) {
		top_left -= p1;
		bottom_right += p1;
		return *this;
	}
	rectangle& inflate(int i) {
		top_left.x -= i;
		top_left.y -= i;
		bottom_right.x += i;
		bottom_right.y += i;
		return *this;
	}
	rectangle& deflate(point p1) {
		top_left += p1;
		bottom_right -= p1;
		return *this;
	}
	rectangle& deflate(int i) {
		top_left.x += i;
		top_left.y += i;
		bottom_right.x -= i;
		bottom_right.y -= i;
		return *this;
	}
	rectangle& width(int i) {
		bottom_right.x = top_left.x + i;
		return *this;
	}
	rectangle& height(int i) {
		bottom_right.y = top_left.y + i;
		return *this;
	}
	bool contains(const point& p1) const {
		return p1.y >= top_left.y && p1.y < bottom_right.y && p1.x >= top_left.x && p1.x < bottom_right.x;
	}
	point top_left;
	point bottom_right;
};

//equality
bool operator==(const point& p1, const point& p2) {
	return p1.x == p2.x && p1.y == p2.y;
}
class chess_board;

class chess_piece
{
public:
    enum color_type { white, black };

    chess_piece( point const& pos, color_type, chess_board& );

    point position() const { return pos_; }
    std::string position_string() const;
    color_type color() const { return color_; }

    void move_to( point const& );

    virtual std::string name() const { assert( false ); return ""; }
    virtual std::string short_name() const { assert( false ); return ""; } 
    virtual bool is_valid_move( point const& ) const;

private:
    point pos_;
    color_type const color_;
    chess_board& board_;
};


class chess_board
{
public:
    bool is_within_bounds( point const& pos ) const
    {
        return bounds_.contains( pos );
    }

private:
    rectangle const bounds_{ 1, 1, 9, 9 };    
};


chess_piece::chess_piece( point const& pos, color_type color, chess_board& board )
    : pos_( pos ), color_( color ), board_( board )
{}

auto chess_piece::position_string() const -> std::string
{
    std::ostringstream ss;
    ss << static_cast<char>( 'a' + pos_.x - 1 ) << pos_.y;
    return ss.str();
}


void chess_piece::move_to( point const& pos )
{
    assert( is_valid_move( pos ) );
    pos_ = pos;
}


auto chess_piece::is_valid_move( point const& pos ) const -> bool
{
    return board_.is_within_bounds( pos );
}


class rook : public chess_piece
{
public:
    rook( point const& pos, color_type color, chess_board& board )
        : chess_piece( pos, color, board )
    {}

    // TODO: override chess_piece's default behavior to make the tests pass
	std::string name() const override{ assert(true); return "rook"; }
	bool is_valid_move(point const& p1) const override {
		point p2 = position();
		//rook moves on x or y axis not both at once and position must be on the board
		return (p1.y - p2.y == 0 || p1.x - p2.x == 0) && chess_piece::is_valid_move(p1);
	}
	std::string short_name() const override { assert(true); return "R"; }
};


class bishop : public chess_piece
{
public:
    bishop( point const& pos, color_type color, chess_board& board )
        : chess_piece( pos, color, board )
    {}

    // TODO: override chess_piece's default behavior to make the tests pass
	std::string name() const override { assert(true); return "bishop"; }
	bool is_valid_move(point const& p1) const override {
		point p2 = position();
		//Since a bishop can only move diagonals the deltaX and deltaY must be equal. and position must be on the board
		return std::abs(p1.x - p2.x) == std::abs(p1.y - p2.y) && chess_piece::is_valid_move(p1);
	}
	std::string short_name() const override { assert(true); return "B"; }
};


void record_move( chess_piece& piece, point const& new_pos, std::ostream& out )
{
    out << piece.short_name() << piece.position_string();
    piece.move_to( new_pos );
    out << " " << piece.short_name() << piece.position_string();
}


TEST_CASE( "chess basics" )
{
    chess_board board;

    SUBCASE( "rook" )
    {
        rook r( point( 1, 1 ), chess_piece::white, board );

        REQUIRE( r.name() == "rook" );
        REQUIRE( r.position() == point( 1, 1 ) );
        REQUIRE( r.position_string() == "a1" );
        REQUIRE( r.color() == chess_piece::white );

        REQUIRE( r.is_valid_move( point( 1, 5 ) ) );
        REQUIRE( !r.is_valid_move( point( 5, 5 ) ) );
        REQUIRE( !r.is_valid_move( point( 1, 10 ) ) );
        REQUIRE( !r.is_valid_move( point( 1, 0 ) ) );

        r.move_to( point( 1, 7 ) );
        REQUIRE( r.position() == point( 1, 7 ) );
        REQUIRE( r.position_string() == "a7" );

        std::ostringstream ss;
        record_move( r, point( 7, 7 ), ss );
        REQUIRE( ss.str() == "Ra7 Rg7" );
    }

    SUBCASE( "bishop" )
    {
        bishop b( point( 3, 8 ), chess_piece::black, board );

        REQUIRE( b.name() == "bishop" );
        REQUIRE( b.position() == point( 3, 8 ) );
        REQUIRE( b.position_string() == "c8" );
        REQUIRE( b.color() == chess_piece::black );

        REQUIRE( b.is_valid_move( point( 1, 6 ) ) );
        REQUIRE( !b.is_valid_move( point( 4, 6 ) ) );
        REQUIRE( !b.is_valid_move( point( 0, 5 ) ) );
        REQUIRE( !b.is_valid_move( point( 4, 9 ) ) );

        b.move_to( point( 2, 7 ) );
        REQUIRE( b.position() == point( 2, 7 ) );
        REQUIRE( b.position_string() == "b7" );

        std::ostringstream ss;
        record_move( b, point( 8, 1 ), ss );
        REQUIRE( ss.str() == "Bb7 Bh1" );
    }
}
