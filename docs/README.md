# **ESP32-CAM-ML**

<!-- Translated from Polish by GPT-4o -->

> [Po Polsku](README_pl.md) | [English](README.md)

The ESP32-CAM-ML project is a versatile development platform utilizing the ESP32S3-Sense camera module for image capture and object detection functionalities. The device offers the capability to operate in various network modes, including AP (Access Point) and STA (Station), providing flexibility in network configuration. Through a simple user interface, network parameters, OTA updates, and imaging and sensor settings can be configured.

## Features

### Main Settings

- **Start/Stop Object Detection**: Buttons to start or stop object detection.

### WiFi Configuration

- **Mode**: Option to choose between AP (Access Point) and STA (Station) modes.
- **Timeout**: Connection timeout in milliseconds.
- **Hostname**: Network name for the ESP32-CAM device.
- **Security**: Choice of security protocol (e.g., WPA, WPA2 PSK).

### AP (Access Point) Configuration

- **Ssid**: SSID name for AP mode.
- **Pass**: Password for AP network.
- **Local_ip**: IP address of the ESP32-CAM in AP mode.
- **Gateway**: Gateway IP address for the AP network.
- **Subnet**: Subnet mask for the AP network.

### STA (Station) Configuration

- **Ssid**: SSID name for STA mode.
- **Pass**: Password for the STA network.
- **Dhcp**: Enable/disable DHCP.
- **Local_ip**: Assigned IP address in STA mode.
- **Gateway**: Gateway IP address for the STA network.
- **Subnet**: Subnet mask for the STA network.
- **DNS1**: DNS server address.
- **DNS2**: DNS server address.

### Camera Settings

- **Pixel_format**: Camera image format (e.g., JPEG).
- **Frame_size**: Camera output resolution (e.g., SVGA).
- **Jpeg_quality**: JPEG image compression quality.

### Sensor Settings

- **Framesize**: Specifies the frame size of the image.
- **Brightness**: Image brightness adjustment.
- **Contrast**: Contrast setting.
- **Saturation**: Color saturation setting.
- **Sharpness**: Image sharpness setting.
- **Denoise**: Noise reduction feature.
- **Gainceiling**: Maximum gain setting.
- **Quality**: Image quality scale.
- **Colorbar**: Enable/disable color bar for calibration.
- **Whitebal**: Enable/disable white balance.
- **Gain_ctrl**: Image gain control.
- **Exposure_ctrl**: Exposure control.
- **Hmirror**: Horizontal image mirroring.
- **Vflip**: Vertical image flip.
- **Aec2**: Advanced exposure control.
- **Awb_gain**: Automatic white balance gain.
- **Agc_gain**: Automatic gain control gain.
- **Aec_value**: Exposure control value.
- **Special_effects**: Special effects, e.g., sepia, negative.
- **Wb_mode**: White balance mode.
- **Ae_level**: Automatic exposure level.
- **Dcw**: Digital image cropping.
- **Bpc**: Dead pixel correction.
- **Wpc**: White pixel correction.
- **Raw_gma**: Gamma curve correction in RAW format.
- **Lenc**: Lens chromatic aberration correction.

## Technologies

### Frontend

- **HTML**: Page structure built using HTML (`index.html`, `api.html`).
- **CSS**: Styling achieved with CSS (`style.css`).
- **JavaScript**: Scripts (`script.js`) written in JavaScript, additional configuration (`jsconfig.json`) indicates the use of JavaScript in the project.
- **TypeScript**: Usage suggested by the `ml5.d.ts` file in the `types` directory and `tsconfig.json`.
- **Node.js and npm**: Project dependency management using npm, seen in `package.json` and `package-lock.json`.
- **Fonts**: Font files (`Roboto-Bold.ttf`, `Roboto-Regular.ttf`) in the `assets` directory used in the project.
- **Developer Tools**:
  - Visual Studio Code (`.vscode`): Specific configurations for VS Code with files `launch.json`, `settings.json`, `tasks.json`.
  - Automation and building (`make_bundle`): Use of Go in build scripts (`go.mod`, `go.sum`, `make_bundle.go`).
- **Testing**: Directory structure suggests the project includes tests (`test.html`, `tests.js`).

### Backend

- **C++**: Programming language used for source files (`app.cpp`, `main.cpp`, `ota.cpp`, `stream.cpp`), indicating C++ is used for application logic implementation.
- **Tools and Libraries**:
  - **VS Code** (`extensions.json` in `.vscode` folder): Using Visual Studio Code as the development environment with specific extensions.
  - **Libraries** (`lib`):
    - The `led` directory may contain specific libraries or modules for LED control.
- **Header Files** (`include`):
  - Header files (`app.hpp`, `config.hpp`, `error.hpp`, `json.hpp`, `ota.hpp`, `stream.hpp`) may define interfaces and common data structures for project modules.
- **Auxiliary Directories**:
  - **Data** (`data`):
    - Contains `settings.jsonc`, which may signify environment-specific or application-specific configuration.
  - **Tools and Types** (`tools`, `types`):
    - These directories may contain auxiliary tools and data type definitions used in the project.

## Installation

**Programs:**

- Install [Visual Studio Code](https://code.visualstudio.com/download)

**To install and run the application locally, follow these steps:**

> Wait for each step to complete before proceeding

1. Clone the repository:

    ```bash
    git clone https://github.com/playday3008/ESP32-CAM-ML.git
    cd ESP32-CAM-ML
    ```

2. Open the code in VS Code.
3. Click "Open Workspace".
4. Connect the controller to the computer.
5. Install recommended extensions by VS Code/repository.
6. After installing extensions, perform the steps:
    - "Terminal" --> "Run Task..." --> run all "Get Compressed..." tasks.
    - On the activity bar, click "PlatformIO" (alien icon).
        - Select "seed_xiao_esp32s3".
        - In the "Platform" folder, click "Upload Filesystem Image".
        - In the "General" folder, click "Upload".

## Authors

- **Vadym Tytan** - *Backend* - [PlayDay](https://github.com/playday3008)
- **Aleksandra Jasnosz** - *Frontend* - [Olala53](https://github.com/Olala53)
- **Bartosz Hawliczek** - *Tests* - [Jacksin76](https://github.com/Jacksin76)
- **Krzysztof Kurzdym** - *Docs* - [Bismarck416](https://github.com/Bismarck416)

## How to Contribute to the Project

We encourage reporting bugs, proposing features, and creating pull requests.
