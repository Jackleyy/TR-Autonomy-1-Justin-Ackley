#include "spin_slow_update.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

SlowSolution::SlowSolution() : Node("slowsolution") {
  // RCLCPP_INFO(this->get_logger(), "Remove this statement from
  // spin_slow_update.cpp"); set our publisher to publish our prediction topic
  //  10 is for how many messegse can be kept in queue
  pubby = this->create_publisher<ArrayMsg>("/predictedpos", 10);

  subby = this->create_subscription<ArrayMsg>(
      "measuredpos", // name of subscription
      10,            // queue length
      // bind connects our class function(pos_callback) to the sub?
      std::bind(&SlowSolution::callBack, this, std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(), "SlowSolution node activated");
}

void SlowSolution::callBack(const ArrayMsg::SharedPtr msg) {
  // msg is one of our smart pointers
  // this will access msg, then publish it to pubby
  pubby->publish(*msg);
}

// your code here