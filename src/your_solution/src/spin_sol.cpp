#include "spin_sol.h"
#include <memory>
#include <functional>
#include <chrono>

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

// your code here
SpinSolution::SpinSolution() : Node("spinsolution") {
  //initialize our values
  pos_x = 0.0;
  vel_x = 0.0;

  pos_y = 0.0;
  vel_y = 0.0; 


  //start with false bc nothinng it read yet
  has_p = false;
  has_v = false;

  // make sure there is room in our vector to store these elements
  predicted_pos.data.resize(2);


  RCLCPP_INFO(this->get_logger(), "Node is running");

  // create a publsher to publish our predicted position
  pubby = this->create_publisher<ArrayMsg>("/predictedpos", 10);

  // create subscribers to catch spinny's measurements
  pos_subby = this->create_subscription<ArrayMsg>(
    "/measuredpos",
    10,
    std::bind(&SpinSolution::pos_clanker, this, std::placeholders::_1)
  );

  vel_subby = this->create_subscription<ArrayMsg>(
    "/measuredvel",
    10,
    std::bind(&SpinSolution::vel_clanker, this, std::placeholders::_1)
  );

  //now call the prediction with a timer
  
  timer_ = create_wall_timer(
    std::chrono::milliseconds(1), 
    std::bind(&SpinSolution::predict, this)
  );

  
}


//this will receive our velocity and then update it
void SpinSolution::vel_clanker (const ArrayMsg::SharedPtr msg) {
  if((msg->data.size() < 2)){
    return;
  }
  vel_x = msg->data[0]; //x velocity
  vel_y = msg->data[1]; //y velocity
  has_v = true; 



}

//same thing as above but for position
void SpinSolution::pos_clanker (const ArrayMsg::SharedPtr msg) {
  if((msg->data.size() < 2)){
    return;
  }
  pos_x = msg->data[0]; //x pos
  pos_y = msg->data[1]; //y pos
  has_p = true; //we now have some values of
  
  //lets us know when the last update was
  then = this->now();

}

void SpinSolution::predict(){
  if(!(has_p && has_v)){
    return;
  }
  dt = this->now() - then;
  predicted_pos.data[0] = pos_x + (dt.seconds()*vel_x);
  predicted_pos.data[1] = pos_y + (dt.seconds()*vel_y);

  pubby->publish(predicted_pos);
}

