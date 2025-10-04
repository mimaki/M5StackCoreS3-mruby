#include "driver/usb_serial_jtag.h"
#include "usbserial.h"

extern "C" {

#define USB_SERIAL_TX_BUFFER_SIZE 256
#define USB_SERIAL_RX_BUFFER_SIZE 256

// Initialize USB Serial
void usb_serial_init(void)
{
  const usb_serial_jtag_driver_config_t cfg = {
    .tx_buffer_size = USB_SERIAL_TX_BUFFER_SIZE,
    .rx_buffer_size = USB_SERIAL_RX_BUFFER_SIZE
  };
  ESP_ERROR_CHECK(usb_serial_jtag_driver_install((usb_serial_jtag_driver_config_t*)&cfg));
}

// Send a byte via USB Serial
void usb_serial_send_byte(uint8_t b)
{
  usb_serial_jtag_write_bytes(&b, 1, 0);
}

// Send multiple bytes via USB Serial
void usb_serial_send_bytes(uint8_t *b, size_t len)
{
  usb_serial_jtag_write_bytes(b, len, 0);
}

// Receive a byte via USB Serial, return -1 if no data
int16_t usb_serial_receive_byte(void)
{
  uint8_t b;
  size_t r = usb_serial_jtag_read_bytes(&b, 1, 0);
  return r == 1 ? b : -1;
}

// Receive bytes via USB Serial
size_t usb_serial_receive_bytes(uint8_t *b, size_t len)
{
  return usb_serial_jtag_read_bytes(b, len, 0);
}

} // extern "C"
