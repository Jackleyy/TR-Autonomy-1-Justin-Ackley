#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
public:
  SpinSolution();

private:
  // your code here

  // declare our publisher
  rclcpp::Publisher<ArrayMsg>::SharedPtr pubby;

  // same for our position subscriber
  rclcpp::Subscription<ArrayMsg>::SharedPtr pos_subby;
  
  //declare velocity subscriber
  rclcpp::Subscription<ArrayMsg>::SharedPtr vel_subby;

  // make the function to be called anytime a message is received
  void pos_clanker(const ArrayMsg::SharedPtr msg);
  void vel_clanker(const ArrayMsg::SharedPtr msg);

  //Now we need somewhere to store both of those things
  double pos_x;
  double vel_x;

  double pos_y;
  double vel_y;

  //bools to check if pos has been recorded
  bool has_p;
  bool has_v;

  //Time object so we can get dt
  rclcpp::Time then;
  rclcpp::Duration dt{0,0};//must initialize to zero because the default constructor is wack

  //and a function to calculate them together
  void predict();

  //and of course somewhere to store the preditciotn
  ArrayMsg predicted_pos;

  //finallyt our timer to call predict
  rclcpp::TimerBase::SharedPtr timer_;
};

#endif // YOUR_SOLUTION_SRC_SPIN_SOL_H_
