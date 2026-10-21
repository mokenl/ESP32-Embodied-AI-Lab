# Firmware · Arduino 环境说明

本目录存放 `01-smart-sensor-node` 模块的 Arduino 固件。

---

## 一、环境已配置内容

| 组件 | 版本 | 位置 |
| --- | --- | --- |
| Arduino IDE | 2.3.10 | `%LOCALAPPDATA%\Microsoft\WinGet\Packages\ArduinoSA.IDE.stable_*` |
| Arduino CLI | 1.5.1 | `C:\Program Files\Arduino CLI\arduino-cli.exe` |
| ESP32 核心 | 3.3.12 | `%LOCALAPPDATA%\Arduino15\packages\esp32` |
| 配置目录 | — | `%LOCALAPPDATA%\Arduino15` |
| Sketchbook | — | `D:\Documents\Arduino` |

ESP32 核心含全系列芯片工具链：ESP32 / S2 / S3 / C3 / C5 / C6 / H2 / P4。

---

## 二、开发板索引（重要）

国内直连 GitHub 无法下载工具链，因此 `board_manager.additional_urls` 指向一份**本地镜像索引**：

```text
file:///C:/Users/Administrator/AppData/Local/Arduino15/package_esp32_index_cn.json
```

这份文件是把官方 `package_esp32_index.json` 里 1247 个 GitHub 下载地址改写成 `gh-proxy.com` 代理后的副本。Arduino IDE 与 Arduino CLI 共用它。

**后果**：安装 / 升级 ESP32 核心可以正常进行；但索引是快照，不会自动跟随官方更新。

### 想升级到新版核心时

```bash
# 1. 重新下载官方索引
curl -L -o "%LOCALAPPDATA%\Arduino15\package_esp32_index_official.json" \
  https://espressif.github.io/arduino-esp32/package_esp32_index.json

# 2. 重写为代理地址（脚本见 output/update_esp32_index.py）

# 3. 刷新并升级
arduino-cli core update-index
arduino-cli core upgrade esp32:esp32
```

### 想改回官方直连（有稳定国际网络时）

```bash
arduino-cli config set board_manager.additional_urls \
  https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core update-index
```

同时把 `%USERPROFILE%\.arduinoIDE\arduino-cli.yaml` 和 `settings.json` 里的 URL 一起改掉，否则 IDE 仍走本地镜像。

---

## 三、命令行用法（推荐，可脚本化）

```bash
# 查看已连接的开发板与串口
arduino-cli board list

# 编译（以经典 ESP32 为例）
arduino-cli compile --fqbn esp32:esp32:esp32 blink_test

# 烧录（COM 口按实际填）
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32 blink_test

# 打开串口监视器（115200 波特率）
arduino-cli monitor -p COM3 -c baudrate=115200
```

常用 FQBN：

| 芯片 | FQBN |
| --- | --- |
| ESP32（经典 DevKit） | `esp32:esp32:esp32` |
| ESP32-S3 | `esp32:esp32:esp32s3` |
| ESP32-C3 | `esp32:esp32:esp32c3` |
| ESP32-S2 | `esp32:esp32:esp32s2` |
| ESP32-C6 | `esp32:esp32:esp32c6` |

烧录后若一直停在 `Connecting........_____`，按住开发板上的 **BOOT** 键再试一次（部分板子需要手动进下载模式）。

---

## 四、Arduino IDE 用法

1. 双击桌面或开始菜单的 **Arduino IDE**
2. 打开本目录下的 `blink_test/blink_test.ino`
3. 顶部下拉框选择开发板：**Tools → Board → esp32 → 你的型号**
4. 选择串口：**Tools → Port → COMx**
5. 点 **→** 上传

首次打开时若提示安装 USB 驱动或选择开发板，按提示走即可。

---

## 五、blink_test 说明

`blink_test/blink_test.ino` 是环境验证程序，做三件事：

1. 串口打印芯片身份（型号 / 核心数 / 主频 / Flash / 堆内存 / MAC / SDK 版本）
2. 板载 LED 每 500ms 翻转
3. 每 2 秒打印一次心跳

板载 LED 引脚已按芯片自动选择。**注意 ESP32-S3-DevKitC-1 的板载灯是 GPIO48 上的 WS2812 可寻址灯珠，不是普通 LED**——`digitalWrite()` 点不亮它，必须用 `rgbLedWrite()`。程序里已用 `setLed()` 做了封装，两种板子都能跑。

**验证通过的标准**：串口能看到 `芯片型号 : ESP32-S3` 之类的信息，且板载 RGB 灯在闪绿光。

---

## 六、ESP32-S3-DevKitC-1 专属说明

这块板子有**两个 USB 口**，用途不同：

| 口 | 丝印 | 芯片 | 驱动 | 适用 |
| --- | --- | --- | --- | --- |
| 左 | `UART` | CP2102N | 需 CP210x 驱动（Win10/11 通常自动装） | **首次烧录推荐**，最稳 |
| 右 | `USB` | 原生 USB-Serial-JTAG | **免驱**（系统自带 `usbser.sys`） | 免驱调试、USB CDC 串口 |

### 推荐流程（用 UART 口）

1. 数据线插**左边 `UART` 口**
2. `arduino-cli board list` 应看到 `COMx  esp32  ...`
3. 烧录：

```bash
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32s3 blink_test
arduino-cli monitor -p COM3 -c baudrate=115200
```

### 用 USB 口时（免驱，但要多一个设置）

原生 USB 口的串口输出依赖 USB CDC，必须在编译参数里打开，否则串口监视器收不到任何东西：

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3:CDCOnBoot=cdc blink_test
arduino-cli upload  -p COMx --fqbn esp32:esp32:esp32s3:CDCOnBoot=cdc blink_test
```

IDE 里对应的是 **Tools → USB CDC On Boot → Enabled**。

### 进不了下载模式怎么办

烧录时若一直卡在 `Connecting........_____`：

1. 按住板上的 **BOOT** 键不放
2. 按一下 **RST**（或拔插 USB）
3. 松开 BOOT
4. 立刻重试烧录

### 如果 UART 口插上后没有 COM 口

说明 CP210x 驱动没装上。两种办法：

- **等 Windows Update 自动装**（插上后联网几分钟，多数情况会自己好）
- **手动装**：浏览器打开 `https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers`，下载 `CP210x Universal Windows Driver`，解压后右键 `silabser.inf` → 安装

**临时绕过**：直接改用右边的 `USB` 口（免驱），按上一节的 `CDCOnBoot=cdc` 参数烧录即可。

---

## 七、串口驱动通用对照

ESP32 开发板通过 USB 转串口芯片与电脑通信，常见三种：

| 芯片 | 说明 | 驱动 |
| --- | --- | --- |
| CP2102 / CP2104 / CP2102N | Silicon Labs，DevKitC 系列常见 | 需装 CP210x 驱动 |
| CH340 / CH9102 | 沁恒，多数国产板 | 需装 CH341SER 驱动 |
| 原生 USB | ESP32-S2/S3/C3 直连 | Win10/11 自带 `usbser.sys`，免驱 |

**判断方法**：插上开发板后运行

```bash
arduino-cli board list
```

- 出现 `COM3  esp32  ...` → 驱动已就绪
- 显示 `Unknown` 或什么都没出现 → 需要装驱动，去设备管理器看是否有带黄色感叹号的设备
