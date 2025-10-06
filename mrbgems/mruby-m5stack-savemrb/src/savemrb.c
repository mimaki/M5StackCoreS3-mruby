#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/data.h"
// #include "../../../main/usbserial.h"
#include "M5CUtil.h"
#include "stdbool.h"
#include <stdio.h>
#include <sys/stat.h>  // mkdir


#ifndef MOUNT_POINT
#define MOUNT_POINT "/sdcard"
#endif

static mrb_value savemrb(mrb_state *mrb, mrb_value self);

static mrb_value savemrb(mrb_state *mrb, mrb_value self)
{
  bool result = false;

  // filename
  char *fname;
  mrb_get_args(mrb, "z", &fname);  //引数取得

  static char savepath[256];
  sprintf(savepath, "%s/M5MRB", MOUNT_POINT);
  mkdir(savepath, 0777);
  sprintf(savepath, "%s/M5MRB/%s.mrb", MOUNT_POINT, fname);
  m5printf("savepath=%s\n", savepath);

  FILE* save_fp = fopen(savepath, "wb");
  if (!save_fp) {
    m5printf("File open error.\n");
    return mrb_false_value();
  }

  //
  // XMODEM
  //  
  usb_serial_init();
  
  bool nak_1st = true;
  int16_t rx_byte = 0;
  static uint8_t rx_buf[256];
  const uint8_t SOH = 0x01;
  const uint8_t EOT = 0x04;
  const uint8_t ACK = 0x06;
  const uint8_t NAK = 0x15;
  const uint8_t CAN = 0x18;
  do {
    retry_rx_packet:
    if (nak_1st) {
      usb_serial_send_byte(NAK);
      // M5.Lcd.printf("NAK "); M5.update();
      // vTaskDelay(1000 / portTICK_PERIOD_MS);
      m5delay(1000);
    }

    // Wait header
    do {
      rx_byte = usb_serial_receive_byte();
      if (rx_byte >= 0) {
        // OK, next step
        break;
      } else {
        // Timeout, retry
        // M5.Lcd.printf("TO "); M5.update();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        m5delay(1000);
        goto retry_rx_packet;
      }
    } while (true);

    if (rx_byte == SOH) {
      // OK, next step
      // M5.Lcd.printf("SOH "); M5.update();
    } else if (rx_byte == EOT) {
      // M5.Lcd.printf("EOT "); M5.update();
      // TX end, send ACK
      usb_serial_send_byte(ACK);
      // M5.Lcd.printf("ACK "); M5.update();
      break; // All end
    } else if (rx_byte == CAN) {
      m5printf("Canceled.\n");
      goto err_exit;
    } else {
      // Unknown header, ignore
      continue;
    }

    nak_1st = false;

    // block no pair + body + sum (131 bytes)
    for (int i=0; i<131; i++) {
      do {
        rx_byte = usb_serial_receive_byte();
        if (rx_byte >= 0) {
          rx_buf[i] = (uint8_t)rx_byte;
          break;
        } else {
          // Timeout, retry
          // M5.Lcd.printf("TO "); M5.update();
          // vTaskDelay(1000 / portTICK_PERIOD_MS);
          m5delay(1000);
          continue;
        }
      } while (true);
    }

    if (save_fp && (fwrite(rx_buf+2, 128, 1, save_fp) != 1)) {
      m5printf("File write error.\n");
      goto err_exit;
    }

    // Block end
    usb_serial_send_byte(ACK);
    // M5.Lcd.printf("ACK "); M5.update();
  } while (true);

  result = true;

err_exit:

  if (save_fp) {
    fclose(save_fp);
  }

  // UART
  m5printf("XMODEM: end.\n");

  return result ? mrb_true_value() : mrb_false_value();  
}

// Initialize and bind the C function to mruby
void mrb_mruby_m5stack_savemrb_gem_init(mrb_state* mrb) {
  mrb_define_method(mrb, mrb->kernel_module, "savemrb", savemrb, MRB_ARGS_REQ(1));
}

void mrb_mruby_m5stack_savemrb_gem_final(mrb_state* mrb) {
    // cleanup if needed
}
