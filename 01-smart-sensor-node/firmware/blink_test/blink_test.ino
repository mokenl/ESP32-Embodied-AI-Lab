/*
 * blink_test.ino —— ESP32 环境验证程序
 *
 * 用途：确认「工具链 + 开发板 + 串口」这条链路完整可用。
 * 它做三件事：
 *   1. 开机在串口打印芯片身份信息（型号 / 核心数 / 主频 / Flash / MAC / SDK 版本）
 *   2. 板载 LED 每 500ms 翻转一次
 *   3. 每 2 秒在串口打印一次心跳计数
 *
 * 烧录：arduino-cli upload -p COMx --fqbn esp32:esp32:esp32s3 blink_test
 * 监视：arduino-cli monitor -p COMx -c baudrate=115200
 */

// ---------- 板载 LED 引脚：不同开发板不一样 ----------
// CONFIG_IDF_TARGET_* 由 arduino-esp32 在编译时按所选开发板自动定义
#if defined(CONFIG_IDF_TARGET_ESP32S3)
  #define LED_PIN 48        // ESP32-S3-DevKitC-1：GPIO48 上的 WS2812
  #define LED_IS_RGB 1
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  #define LED_PIN 8         // ESP32-C3-DevKitM-1：GPIO8 上的 WS2812
  #define LED_IS_RGB 1
#elif defined(CONFIG_IDF_TARGET_ESP32C6)
  #define LED_PIN 8
  #define LED_IS_RGB 1
#else
  #define LED_PIN 2         // 经典 ESP32 DevKit：GPIO2 普通 LED
  #define LED_IS_RGB 0
#endif

const unsigned long BLINK_INTERVAL = 500;       // LED 翻转间隔（毫秒）
const unsigned long HEARTBEAT_INTERVAL = 2000;  // 心跳间隔（毫秒）

unsigned long lastBlink = 0;
unsigned long lastBeat = 0;
unsigned long beatCount = 0;
bool ledState = false;

// 统一的点灯接口：RGB 灯珠和普通 LED 的驱动方式完全不同
void setLed(bool on) {
#if LED_IS_RGB
  // WS2812 是可寻址灯珠，必须用专门函数，digitalWrite 点不亮
  if (on) {
    rgbLedWrite(LED_PIN, 0, 40, 0);   // 暗绿色，避免刺眼
  } else {
    rgbLedWrite(LED_PIN, 0, 0, 0);    // 熄灭
  }
#else
  digitalWrite(LED_PIN, on ? HIGH : LOW);
#endif
}

void setup() {
  Serial.begin(115200);
  delay(300);   // 等串口稳定，否则前几行会丢

  Serial.println();
  Serial.println("========================================");
  Serial.println("  ESP32 环境验证 · blink_test");
  Serial.println("========================================");

  // ---------- 打印芯片身份 ----------
  Serial.printf("芯片型号   : %s\n", ESP.getChipModel());
  Serial.printf("芯片版本   : v%d.%d\n", ESP.getChipRevision() / 100,
                                       ESP.getChipRevision() % 100);
  Serial.printf("核心数     : %d\n", ESP.getChipCores());
  Serial.printf("主频       : %lu MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash 大小 : %lu KB\n", ESP.getFlashChipSize() / 1024);
  Serial.printf("可用堆内存 : %lu KB\n", ESP.getFreeHeap() / 1024);
  Serial.printf("SDK 版本   : %s\n", ESP.getSdkVersion());

  // MAC 存在 eFuse 里，是个 64 位整数，要按 6 个字节展开才是常见写法
  uint64_t mac = ESP.getEfuseMac();
  Serial.printf("MAC 地址   : %02X:%02X:%02X:%02X:%02X:%02X\n",
                (uint8_t)(mac >> 40), (uint8_t)(mac >> 32), (uint8_t)(mac >> 24),
                (uint8_t)(mac >> 16), (uint8_t)(mac >> 8),  (uint8_t)(mac));
  Serial.println("----------------------------------------");
#if LED_IS_RGB
  Serial.printf("板载 LED   : GPIO%d（WS2812 可寻址灯珠）\n", LED_PIN);
#else
  Serial.printf("板载 LED   : GPIO%d（普通 LED）\n", LED_PIN);
#endif
  Serial.println("如果这行之后 LED 开始闪烁，说明环境已就绪。");
  Serial.println("========================================");
  Serial.println();

#if !LED_IS_RGB
  pinMode(LED_PIN, OUTPUT);
#endif
  setLed(false);
}

void loop() {
  unsigned long now = millis();

  // ---------- LED 闪烁 ----------
  if (now - lastBlink >= BLINK_INTERVAL) {
    lastBlink = now;
    ledState = !ledState;
    setLed(ledState);
  }

  // ---------- 串口心跳 ----------
  if (now - lastBeat >= HEARTBEAT_INTERVAL) {
    lastBeat = now;
    beatCount++;
    Serial.printf("[心跳 %lu] 运行 %lu 秒 | LED=%s | 剩余内存 %lu KB\n",
                  beatCount,
                  now / 1000,
                  ledState ? "ON " : "OFF",
                  ESP.getFreeHeap() / 1024);
  }
}
