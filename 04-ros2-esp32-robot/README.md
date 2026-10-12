# 04 · ROS2 + ESP32 机器人（ROS2 + ESP32 Robot）

## 目标

把 ESP32 节点接入 ROS2 生态：ESP32 侧作为 **micro-ROS 节点**发布传感器、订阅控制指令，上位机侧用 ROS2 完成建图、定位与导航。

这是四个模块的收口——感知、推理、执行在这里被统一编排成一个真正的机器人系统。

## 系统架构

```text
┌──────────────── ESP32 (micro-ROS node) ────────────────┐
│  IMU / 编码器 / 里程计  →  publish                     │
│  电机 / 舵机            ←  subscribe                   │
└────────────────────────┬───────────────────────────────┘
                         │ UART / UDP (micro-ROS agent)
┌────────────────────────┴───────────────────────────────┐
│                    上位机 Ubuntu + ROS2                │
│  /odom  /imu  /scan  /cmd_vel  →  SLAM · Nav2 · TF     │
└────────────────────────────────────────────────────────┘
```

## 硬件清单

| 器件 | 说明 |
| --- | --- |
| ESP32 / ESP32-S3 | micro-ROS 节点主控 |
| 差速底盘 + 编码器电机 | 里程计来源 |
| IMU（MPU6050 / BNO055） | 姿态与角速度 |
| 激光雷达（可选，如 RPLIDAR） | 建图与避障 |
| 上位机 | Ubuntu 22.04 / 24.04 |

## 技术栈

- **固件侧**：micro-ROS、FreeRTOS、uros 传输（串口 / UDP）
- **上位机侧**：ROS2 Humble / Jazzy、`micro_ros_agent`、`robot_state_publisher`、`tf2`
- **算法侧**：里程计推算、EKF 传感器融合（`robot_localization`）、SLAM（`slam_toolbox`）、Nav2
- **调试工具**：`rqt`、`rviz2`、`ros2 topic` / `ros2 bag`

## 目录规划

```text
04-ros2-esp32-robot
├── firmware/            # micro-ROS 固件工程
├── ros2_ws/             # ROS2 工作空间 (colcon)
│   └── src/             # 自研功能包：描述、驱动、导航配置
├── launch/              # 启动文件
├── config/              # 参数、URDF / xacro、导航配置
└── docs/                # 话题表、TF 树、标定与联调记录
```

## 话题规划

| 话题 | 类型 | 方向 | 说明 |
| --- | --- | --- | --- |
| `/odom` | `nav_msgs/Odometry` | 发布 | 轮式里程计 |
| `/imu/data` | `sensor_msgs/Imu` | 发布 | IMU 数据 |
| `/cmd_vel` | `geometry_msgs/Twist` | 订阅 | 速度指令 |
| `/scan` | `sensor_msgs/LaserScan` | 发布 | 激光雷达（可选） |
| `/joint_states` | `sensor_msgs/JointState` | 发布 | 关节状态 |

## 里程碑

- [ ] 上位机安装 ROS2，`micro_ros_agent` 跑通
- [ ] ESP32 侧建立 micro-ROS 节点，ping 通 agent
- [ ] 发布 `/odom` 与 `/imu/data`
- [ ] 订阅 `/cmd_vel` 驱动底盘
- [ ] URDF 建模 + RViz2 中 TF 正确显示
- [ ] EKF 融合里程计与 IMU
- [ ] SLAM 建图 + Nav2 自主导航
