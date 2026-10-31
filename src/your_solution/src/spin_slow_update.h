#ifndef YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
#define YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

// SlowS class inherits from Node
class SlowSolution : public rclcpp::Node {
public:
  SlowSolution();

private:
  /*
    create the publisher object
    This guy is part of the rclpp class that we included up at top
    Data type: Publisher vector
    type filled with: custom definition from top: Arraymsg
    SharedPtr is??
    name = pub
  */

  rclcpp::Publisher<ArrayMsg>::SharedPtr pubby;

  // same thing as above but now for our listener
  rclcpp::Subscription<ArrayMsg>::SharedPtr subby;

  // our function whenever a position is received by spinny
  // gotta use scope resolution on SharedPtr because it lives inside Arraymsg
  void callBack(const ArrayMsg::SharedPtr msg);
};

#endif // YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
