# ESP32-Embodied-AI-Lab

基于 ESP32 系列的**具身智能（Embodied AI）**学习与实验项目集。

从单个传感器节点出发，逐步走到边缘 AI 视觉、网络化机器人节点，最终汇入 ROS2 + ESP32 的完整机器人系统。四个模块各自独立、可单独实验，也可逐级组合成一个闭环的具身智能平台。

## 目录结构

```text
ESP32-Embodied-AI-Lab
├── 01-smart-sensor-node      # 智能传感器节点
├── 02-edge-ai-vision         # 边缘 AI 视觉
├── 03-networked-robot-node   # 网络化机器人节点
└── 04-ros2-esp32-robot       # ROS2 + ESP32 机器人
```

## 模块路线

| 序号 | 模块 | 目标 | 关键技术 |
| --- | --- | --- | --- |
| 01 | [smart-sensor-node](./01-smart-sensor-node) | 稳定采集物理量并完成本地预处理与上报 | ESP32 / I2C / SPI / FreeRTOS / 低功耗 |
| 02 | [edge-ai-vision](./02-edge-ai-vision) | 在 MCU 上完成轻量视觉推理 | ESP32-S3 / 摄像头 / ESP-DL / TFLite Micro |
| 03 | [networked-robot-node](./03-networked-robot-node) | 让节点联网，实现远程控制与状态回传 | Wi-Fi / MQTT / ESP-NOW / 电机闭环 |
| 04 | [ros2-esp32-robot](./04-ros2-esp32-robot) | 把 ESP32 接入 ROS2 生态 | micro-ROS / ROS2 / 里程计 / 传感器融合 |

## 能力递进关系

```text
感知(01)  →  理解(02)  →  行动+联网(03)  →  系统集成(04)
```

- **01** 解决「怎么把物理世界变成可靠的数据」。
- **02** 解决「数据够不够聪明」，把推理放到边缘侧。
- **03** 解决「怎么动、怎么连」，加入执行器与网络。
- **04** 解决「怎么成为一个真正的机器人系统」，交给 ROS2 统一编排。

## 硬件与工具链

- **主控**：ESP32 / ESP32-S3 / ESP32-C3
- **开发环境**：Arduino IDE 2.x、ESP-IDF v5.x、PlatformIO
- **上位机**：Ubuntu 22.04 / 24.04 + ROS2 Humble / Jazzy
- **常用协议**：I2C、SPI、UART、ADC、PWM、Wi-Fi、MQTT、micro-ROS

## 进度

- [x] 建立四个模块的目录骨架
- [ ] 01 智能传感器节点：最小采集链路跑通
- [ ] 02 边缘 AI 视觉：板端推理 Demo
- [ ] 03 网络化机器人节点：远程控制闭环
- [ ] 04 ROS2 + ESP32：micro-ROS 话题打通

## 使用说明

每个模块目录下都有独立的 `README.md`，包含目标、硬件清单、技术栈与里程碑，可按需进入对应目录开始实验。
