import curses
import time

import numpy as np

import rclpy
from rclpy.node import Node
from spinnything.util import draw_circle_and_points
from std_msgs.msg import Float64MultiArray

CIRCLE_PERIOD = 10
TERM_REFRESH_PERIOD = 1 / 60
MEASURED_PUBLISH_PERIOD = 1.5

screen = curses.initscr()

SCREENSIZE = min(int(curses.COLS / 2), curses.LINES)
if SCREENSIZE % 2 == 0:
    SCREENSIZE -= 1
HALFWAY = (SCREENSIZE - 1) / 2
CENTER = np.array([HALFWAY, HALFWAY])

CIRCLERADIUS = int(SCREENSIZE * .4)


class CirclePrinter(Node):
    def __init__(self):
        super().__init__('spinnything')
        self.timer = 0
        self.theta = 0
        self.coord2 = np.array([HALFWAY, HALFWAY])
        self.subscription = self.create_subscription(
            Float64MultiArray,
            'predictedpos',
            self.predicted_pos_callback,
            10)
        self.pospublisher = self.create_publisher(Float64MultiArray, 'measuredpos', 10)
        self.velpublisher = self.create_publisher(Float64MultiArray, 'measuredvel', 10)
        self.publishtimer = self.create_timer(MEASURED_PUBLISH_PERIOD, self.timer_callback)
        self.starttime = time.time()

    def update(self):
        self.theta = 2 * np.pi * (time.time() - self.starttime) / CIRCLE_PERIOD
        self.coord1 = self.compute_coord1()
        draw_circle_and_points(screen, self.coord1, self.coord2)
        screen.refresh()
        self.timer += 1

    def compute_coord1(self) -> np.ndarray:
        return CENTER + (CIRCLERADIUS * self.compute_pos())

    def compute_pos(self) -> np.ndarray:
        return np.array([np.cos(self.theta), np.sin(self.theta)])

    def compute_vel(self) -> np.ndarray:
        return (2 * np.pi / CIRCLE_PERIOD) * np.array([-np.sin(self.theta), np.cos(self.theta)])

    def timer_callback(self):
        pos = self.compute_pos()
        vel = self.compute_vel()
        posmsg = Float64MultiArray()
        velmsg = Float64MultiArray()
        posmsg.data = [float(pos[0]), float(pos[1])]
        velmsg.data = [float(vel[0]), float(vel[1])]
        self.pospublisher.publish(posmsg)
        self.velpublisher.publish(velmsg)

    def predicted_pos_callback(self, msg: Float64MultiArray):
        self.coord2 = CENTER + (CIRCLERADIUS * (np.array([msg.data[0], msg.data[1]])))
        self.update()


def main(args=None):
    rclpy.init(args=args)
    printer = CirclePrinter()
    while True:
        try:
            printer.update()
            rclpy.spin_once(printer, timeout_sec=TERM_REFRESH_PERIOD)
        except BaseException as e:
            curses.nocbreak()
            curses.echo()
            curses.endwin()
            raise e
