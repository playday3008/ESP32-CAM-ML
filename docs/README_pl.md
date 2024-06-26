# **ESP32-CAM-ML**

> [English](README.md) | [Po Polsku](README_pl.md)

Projekt ESP32-CAM-ML to wszechstronna platforma rozwojowa wykorzystująca moduł kamery ESP32S3-Sense do przechwytywania obrazów i realizacji funkcji detekcji obiektów. Urządzenie oferuje możliwość pracy w różnych trybach sieciowych, w tym AP (Access Point) i STA (Station), zapewniając elastyczność w konfiguracji sieciowej. Za pomocą prostego interfejsu użytkownika można skonfigurować parametry sieciowe, aktualizacje OTA oraz parametry obrazowania i sensora.

## Funkcje

### Main Settings

- **Start/Stop Object Detection**: Przyciski do rozpoczynania lub zakończenia detekcji obiektów.

### Konfiguracja *WiFi*

- **Mode**: Możliwość wyboru między trybem AP (Access Point) a trybem STA (Station).
- **Timeout**: Czas oczekiwania na połączenie w milisekundach.
- **Hostname**: Nazwa sieciowa dla urządzenia ESP32-CAM.
- **Security**: Wybór protokołu zabezpieczeń (np. WPA, WPA2 PSK).

### Konfiguracja *AP* (Access Point)

- **Ssid**: Nazwa sieci SSID dla trybu AP.
- **Pass**: Hasło dla sieci AP.
- **Local_ip**: Adres IP urządzenia ESP32-CAM w trybie AP.
- **Gateway**: Adres IP bramy dla sieci AP.
- **Subnet**: Maska podsieci dla sieci AP.

### Konfiguracja *STA* (Station)

- **Ssid**: Nazwa sieci SSID dla trybu STA.
- **Pass**: Hasło do sieci w trybie STA.
- **Dhcp**: Włączenie/wyłączenie DHCP.
- **Local_ip**: Przypisany adres IP w trybie STA.
- **Gateway**: Adres IP bramy dla sieci AP.
- **Subnet**: Maska podsieci dla sieci AP.
- **DNS1**: Adres serwera DNS.
- **DNS2**: Adres serwera DNS.

### Ustawienia Kamery *Camera*

- **Pixel_format**: Format obrazu kamery (np. JPEG).
- **Frame_size**: Rozdzielczość wyjścia kamery (np. SVGA).
- **Jpeg_quality**: Jakość kompresji zdjęć JPEG.

### Ustawienia Sensora *Sensor Settings*

- **Framesize**: Określa rozmiar ramki obrazu.
- **Brightness**: Regulacja jasności obrazu.
- **Contrast**: Ustawienie kontrastu.
- **Saturation**: Ustawienie nasycenia barw.
- **Sharpness**: Ustawienie ostrości obrazu.
- **Denoise**: Funkcja redukcji szumów.
- **Gainceiling**: Ustawienie maksymalnego wzmocnienia.
- **Quality**: Jakość obrazu w skali.
- **Colorbar**: Włączenie/wyłączenie paska kolorów do kalibracji.
- **Whitebal**: Włączenie/wyłączenie balansu bieli.
- **Gain_ctrl**: Kontrola wzmocnienia obrazu.
- **Exposure_ctrl**: Kontrola ekspozycji.
- **Hmirror**: Odbicie lustrzane obrazu.
- **Vflip**: Odwrócenie obrazu w pionie.
- **Aec2**: Zaawansowana kontrola ekspozycji.
- **Awb_gain**: Automatyczne wzmocnienie balansu bieli.
- **Agc_gain**: Automatyczne wzmocnienie kontroli wzmocnienia.
- **Aec_value**: Wartość kontroli ekspozycji.
- **Special_effects**: Efekty specjalne, np. sepia, negatyw.
- **Wb_mode**: Tryb balansu bieli.
- **Ae_level**: Poziom automatycznej ekspozycji.
- **Dcw**: Cyfrowe przycinanie obrazu.
- **Bpc**: Korekcja martwych pikseli.
- **Wpc**: Korekcja białych pikseli.
- **Raw_gma**: Korekcja krzywej gamma w formacie RAW.
- **Lenc**: Korekcja aberracji chromatycznej obiektywu.

## Technologie

### Frontend

- **HTML**: Struktura stron jest zbudowana z użyciem HTML (`index.html`, `api.html`).
- **CSS**: Stylizacja jest realizowana za pomocą CSS (`style.css`).
- **JavaScript**: Skrypty (`script.js`) są napisane w JavaScript, dodatkowo konfiguracja (`jsconfig.json`) wskazuje na użycie JavaScript w projekcie.
- **TypeScript**: Użycie TypeScript jest sugerowane przez plik `ml5.d.ts` w katalogu `types` oraz konfigurację `tsconfig.json`.
- **Node.js i npm**: Zarządzanie zależnościami projektu odbywa się za pomocą npm, co widać w plikach `package.json` i `package-lock.json`.
- **Fonty**: W katalogu `assets` znajdują się pliki fontów (`Roboto-Bold.ttf`, `Roboto-Regular.ttf`), które są używane w projektie.
- **Narzędzia deweloperskie**:
  - Visual Studio Code (`.vscode`): Konfiguracje specyficzne dla środowiska VS Code z plikami `launch.json`, `settings.json`, `tasks.json`.
  - Automatyzacja i budowanie (`make_bundle`): Użycie Go w skryptach budowania (`go.mod`, `go.sum`, `make_bundle.go`).
- **Testowanie**: Struktura katalogów sugeruje, że projekt zawiera testy (`test.html`, `tests.js`).

### Backend

- **C++**: Język programowania używany do tworzenia plików źródłowych (`app.cpp`, `main.cpp`, `ota.cpp`, `stream.cpp`), co wskazuje na wykorzystanie C++ w implementacji logiki aplikacji.
- **Narzędzia i biblioteki**:
  - **VS Code** (`extensions.json` w folderze `.vscode`): Wykorzystanie Visual Studio Code jako środowiska programistycznego z konkretnymi rozszerzeniami.
  - **Biblioteki** (`lib`):
    - Katalog `led` może zawierać specyficzne biblioteki lub moduły do obsługi diod LED.
- **Header Files** (`include`):
  - Pliki nagłówkowe (`app.hpp`, `config.hpp`, `error.hpp`, `json.hpp`, `ota.hpp`, `stream.hpp`) mogą definiować interfejsy i wspólne struktury danych dla modułów projektu.
- **Katalogi pomocnicze**:
  - **Data** (`data`):
    - Zawiera `settings.jsonc`, co może oznaczać konfigurację specyficzną dla środowiska lub aplikacji.
  - **Tools and Types** (`tools`, `types`):
    - Katalogi te mogą zawierać narzędzia pomocnicze oraz definicje typów danych używanych w projekcie.

## Instalacja

**Programy:**

- Zainstaluj [Visual Studio Code](https://code.visualstudio.com/download)

**Aby zainstalować i uruchomić aplikację lokalnie, wykonaj poniższe kroki:**

> Po każdym kroku poczekać na wykonanie się poniższych czynności

1. Sklonować repozytorium:

    ```bash
    git clone https://github.com/playday3008/ESP32-CAM-ML.git
    cd ESP32-CAM-ML
    ```

2. Otworzyć kod w VS Code.
3. Kliknąć "Open Workspace".
4. Podłączyć kontroler do komputera.
5. Zainstalować polecane przez VS Code/repozytorium rozszerzenia.
6. Po zainstalowaniu roszrzeżeń wykonać kroki:
    - "Terminal" --> "Run Task..." --> uruchomić wszystkie zadania "Get Compressed...".
    - Na pasku aktywności kliknąć "PlatrformIO" (kosmita).
        - Wybrać "seed_xiao_esp32s3".
        - W folderze "Platform" kliknąć "Upload Filesystem Image".
        - W folderze "General" kliknąć "Upload".

## Autorzy

- **Vadym Tytan** - *Backend* - [PlayDay](https://github.com/playday3008)
- **Aleksandra Jasnosz** - *Frontend* - [Olala53](https://github.com/Olala53)
- **Bartosz Hawliczek** - *Tests* - [Jacksin76](https://github.com/Jacksin76)
- **Krzysztof Kurzdym** - *Docs* - [Bismarck416](https://github.com/Bismarck416)

## Jak przyczynić się do projektu

Zachęcamy do zgłaszania błędów, proponowania funkcji i tworzenia pull requests.
