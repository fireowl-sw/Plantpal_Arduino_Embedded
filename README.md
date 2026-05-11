# Plantpal Arduino Embedded

基于 Arduino Nano RP2040 Connect 的智能植物监控嵌入式系统。通过多种传感器实时采集植物生长环境数据，并经由 WiFi + MQTT 协议上报至云平台。

## 功能特性

- **温度监测** — DHT11 数字温湿度传感器
- **湿度监测** — 环境相对湿度
- **光照监测** — 模拟光照传感器，百分比显示
- **土壤湿度监测** — 模拟土壤湿度传感器，百分比显示
- **自动补光** — 光照低于阈值时自动点亮板载 LED
- **MQTT 数据上报** — 每 30 分钟向 MQTT Broker 推送一次传感器数据

## 硬件

| 组件 | 说明 |
|------|------|
| 主控板 | Arduino Nano RP2040 Connect |
| 温湿度传感器 | DHT11 |
| 光照传感器 | 模拟光照模块 |
| 土壤湿度传感器 | 模拟电容式土壤湿度模块 |

### 引脚接线

| 传感器 | 引脚 |
|--------|------|
| DHT11 数据 | D2 |
| 光照传感器 | A0 |
| 土壤湿度传感器 | A1 |

## 软件依赖

- **PlatformIO** (构建框架)
- **WiFiNINA** ^2.0.1 (WiFi 连接)
- **ArduinoMqttClient** ^0.1.8 (MQTT 通信)
- **DHT Sensor Library** ^1.4.6 (DHT11 驱动)
- **Adafruit Unified Sensor** ^1.1.14 (传感器统一接口)

## 快速开始

### 环境准备

1. 安装 [VS Code](https://code.visualstudio.com/)
2. 安装 [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) 插件
3. 克隆本项目：

```bash
git clone <repository-url>
cd Plantpal_Arduino_Embedded
```

### 配置

在 `src/main.cpp` 中修改以下配置以匹配你的环境：

```cpp
char ssid[] = "your_wifi_ssid";       // WiFi 名称
char pass[] = "your_wifi_password";    // WiFi 密码
```

MQTT Broker 相关配置（默认使用 DFRobot IoT 平台）：

```cpp
const char broker[] = "iot.dfrobot.com";
int port = 1883;
const char mqttUser[] = "your_mqtt_user";
const char mqttPass[] = "your_mqtt_pass";
const char topic[] = "your_topic";
```

### 编译与上传

通过 PlatformIO:

```bash
# 编译
pio run

# 编译并上传到设备
pio run --target upload

# 串口监视器
pio device monitor
```

或通过 VS Code PlatformIO 插件的图形界面操作。

## MQTT 数据格式

每 30 分钟发布一次 JSON 格式的传感器数据：

```json
{
  "light": "45.2%",
  "temp": 25.30,
  "humidity": 60.00,
  "soil": "35.7%"
}
```

| 字段 | 说明 | 单位 |
|------|------|------|
| light | 光照百分比 | % |
| temp | 环境温度 | °C |
| humidity | 环境湿度 | %RH |
| soil | 土壤湿度百分比 | % |

## 项目结构

```
Plantpal_Arduino_Embedded/
├── src/
│   └── main.cpp          # 主程序入口
├── lib/                   # 自定义库
├── include/               # 头文件
├── test/                  # 测试
├── platformio.ini         # PlatformIO 配置
└── README.md
```

## 注意事项

- 数据上报间隔默认为 30 分钟（1800000ms），可在 `publishInterval` 中调整
- ADC 分辨率设置为 12 位（0-4095）
- 光照低于阈值（880）时点亮板载 LED，可在 `darkThreshold` 中调整
- 请勿将 WiFi 密码和 MQTT 凭据提交至公开仓库
