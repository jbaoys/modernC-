/**
 * @file MockTurtleTest.cc
 * @brief Unit tests for MockTurtle
 * @author [Jason Bao](https://github.com/jbaoys)
 */

#include "MockTurtle.h"
#include "Painter.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;                           // #1
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::_;
using ::testing::DoAll;
ACTION_P2(setPoint, x,  y) {
    arg0.x = x;
    arg0.y = y;
    return true;
}

TEST(PainterTest, CanDrawSomething) {
    MockTurtle turtle;                              // #2
    EXPECT_CALL(turtle, PenDown())                  // #3
      .Times(AtLeast(1));
    EXPECT_CALL(turtle, GoTo(_,_)).Times(AtLeast(1));
    EXPECT_CALL(turtle, PenUp()).Times(AtLeast(1));

    Painter painter(turtle, 100, 100);              // #4

    EXPECT_FALSE(painter.DrawCircle(0, 0, 10));      // #5
    EXPECT_TRUE(painter.DrawCircle(10, 0, 10));      // #5
}

TEST(PainterTest, DrawNextStroke) {
    NiceMock<MockTurtle> turtle;

    // Use lambda expression to set point
    EXPECT_CALL(turtle, GetPosition(_)).WillOnce([](Point& p) { p = Point(11, 22); return true; });
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(false));
    Painter painter(turtle, 100, 100);
    EXPECT_FALSE(painter.DrawNextStroke(Point(10,20)));
}

TEST(PainterTest, DrawNextStroke1) {
    NiceMock<MockTurtle> turtle;

    // Use gmock action macro to set point
    EXPECT_CALL(turtle, GetPosition(_)).WillOnce(setPoint(11, 22));
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(false));
    Painter painter(turtle, 100, 100);
    EXPECT_FALSE(painter.DrawNextStroke(Point(10,20)));
}

TEST(PainterTest, DrawNextStroke2) {
    NiceMock<MockTurtle> turtle;

    EXPECT_CALL(turtle, GetPosition(_)).WillOnce(setPoint(11, 22));
    EXPECT_CALL(turtle, GoTo(_,_)).Times(AtLeast(1));
    EXPECT_CALL(turtle, IsPenDown()).WillOnce(Return(true));
    Painter painter(turtle, 100, 100);
    EXPECT_TRUE(painter.DrawNextStroke(Point(10,20)));
}

// int main( int argc, char *argv[] ) {
//     ::testing::InitGoogleMock( &argc, argv );
//     return RUN_ALL_TESTS( );
// }
