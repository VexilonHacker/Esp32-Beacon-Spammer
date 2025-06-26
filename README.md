# Esp32-Beacon-Spammer

A Wi-Fi beacon spammer that uses the ESP32 to broadcast multiple fake SSIDs across selected Wi-Fi channels. Intended for **educational**, **testing**, or **demonstration** purposes only.

> ⚠️ **Disclaimer**: This tool is for **educational and authorized testing** only. Do not use it to interfere with networks you do not own or have explicit permission to test. Misuse may be illegal.

---

## 📌 Features

- 📡 Sends custom SSID beacon frames using raw 802.11 packets.
- 🔒 Supports WPA2 flags in beacon frames.
- 🌀 Rotates across multiple Wi-Fi channels.
- 🧠 Automatically generates padded SSIDs for optimal performance.
- 🧹 Optional trimming of whitespace to keep SSIDs lean.
- 🎲 Uses randomly generated MAC addresses per SSID broadcast.
- 📟 Optional support for OLED screen display using Adafruit SSD1306.

---

## 🛠 Requirements

- **Hardware**:
  - ESP32 development board
  - (Optional) 128x64 OLED display (SSD1306)

- **Software**:
  - Arduino IDE
  - ESP32 Board Package (v2.x recommended)

- **Libraries** (install via Arduino Library Manager or `platformio.ini`):

  ```ini
  me-no-dev/AsyncTCP@^1.1.1
  me-no-dev/ESP Async WebServer@^1.2.4
  FS                // Built-in or platform-specific
  adafruit/Adafruit GFX Library
  adafruit/Adafruit SSD1306
  ```

---

## ⚙️ Setup

1. Clone or download this repository.
2. Open the project in Arduino IDE or PlatformIO.
3. Connect your ESP32 board via USB.
4. Install required libraries.
5. Upload the code to your ESP32.

---

## 🔧 Configuration

### Channels
Set the Wi-Fi channels to use (1–14):
```cpp
const uint8_t channels[] = {1, 6, 11};
```

### WPA2 Beacon Toggle
Enable or disable WPA2 capability flag in beacon:
```cpp
const bool wpa2 = true;
```

### SSID Padding
Append spaces to make SSIDs exactly 32 characters:
```cpp
const bool appendSpaces = true;
```

### Custom SSID List
Edit `ssids[]` to define fake SSIDs (each must end with `\n`):

```cpp
const char ssids[] PROGMEM = {
  "Free WiFi Zone\n"
  "Never Gonna Give You Up\n"
  "404 Network Not Found\n"
  // ... add more here
};
```

**Note**: 
- Max SSID length = 32 characters (including padding)
- No duplicate SSIDs (change at least 1 character)

---

## 🖥 OLED Display (Optional)

If using a 128x64 OLED (SSD1306):
- Connect via I2C to your ESP32
- Include `Adafruit SSD1306` and `Adafruit GFX` libraries
- Customize the code to show broadcast stats or menus on the screen

---

## 📈 Output

- Displays `Packets/s` in the serial monitor.
- Sends beacon frames with updated SSIDs and random MAC addresses.
- Dynamically rotates Wi-Fi channels.

---

## 🧪 Use Cases

- **Education**: Demonstrate Wi-Fi protocol behavior.
- **Training**: Simulate rogue APs for security awareness.
- **Testing**: Load test wireless scanners or Wi-Fi sniffers.

---

## ❗ Legal & Ethical Use

Only operate on networks and frequencies for which you are legally authorized. Interfering with public or private Wi-Fi networks without permission is illegal in many countries.

---

## 📜 License

MIT License — you are free to use, modify, and distribute under the terms of the license. See [LICENSE](LICENSE) file for full details.

---

## 🙌 Credits

Inspired by ESP8266/ESP32-based Wi-Fi deauthers and community security tools. Special thanks to:
- `me-no-dev` for AsyncTCP and AsyncWebServer
- Adafruit for the GFX and SSD1306 libraries

```

