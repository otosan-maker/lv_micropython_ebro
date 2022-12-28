//Board config for Arduino Nano RP2040 Connect.

// Board and hardware specific configuration
#define MICROPY_HW_BOARD_NAME           "Arduino Nano RP2040 Connect"
#define MICROPY_HW_FLASH_STORAGE_BYTES  (8 * 1024 * 1024)

// Enable networking and sockets.
#define MICROPY_PY_NETWORK              (1)
#define MICROPY_PY_USOCKET              (1)

// Enable USB Mass Storage with FatFS filesystem.
#define MICROPY_HW_USB_MSC              (1)
#define MICROPY_HW_USB_VID              (0x2341)
#define MICROPY_HW_USB_PID              (0x015e)

// UART 1 config.
#define MICROPY_HW_UART1_TX             (8)
#define MICROPY_HW_UART1_RX             (9)
#define MICROPY_HW_UART1_CTS            (10)
#define MICROPY_HW_UART1_RTS            (11)

// SPI 1 config.
#define MICROPY_HW_SPI1_SCK             (14)
#define MICROPY_HW_SPI1_MOSI            (11)
#define MICROPY_HW_SPI1_MISO            (8)

// I2C0 config.
#define MICROPY_HW_I2C0_SCL             (13)
#define MICROPY_HW_I2C0_SDA             (12)

// I2C1 config.
#define MICROPY_HW_I2C1_SCL             (27)
#define MICROPY_HW_I2C1_SDA             (26)

// Bluetooth config.
#define MICROPY_HW_BLE_UART_ID          (1)
#define MICROPY_HW_BLE_UART_BAUDRATE    (119600)

// WiFi/NINA-W10 config.
#define MICROPY_HW_WIFI_SPI_ID          (1)
#define MICROPY_HW_WIFI_SPI_BAUDRATE    (8 * 1000 * 1000)

// ublox Nina-W10 module config.
#define MICROPY_HW_NINA_RESET           (3)
#define MICROPY_HW_NINA_GPIO0           (2)
#define MICROPY_HW_NINA_GPIO1           (9)
#define MICROPY_HW_NINA_ACK             (10)
