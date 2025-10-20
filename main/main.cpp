#include <stdio.h>
#include "M5Unified.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdspi_host.h"  // for SD card SPI host driver

#include "M5CUtil.h"

#include <stdio.h>
#include <string.h>

extern "C" {
#define MRB_NO_BOXING   // Should match mruby VM's configuration.
#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/compile.h"
#include "mruby/value.h"
#include "mruby/string.h"
// #include "mruby/presym.h"

extern int mirb(mrb_state *mrb);

#define MOUNT_POINT "/sdcard"
#define MRB_DIR MOUNT_POINT"/M5MRB/"
#define MRB_EXT ".mrb"
#define ONESHOT_INI MOUNT_POINT"/oneshot.ini"
#define APP_MRB MOUNT_POINT"/autorun"MRB_EXT
#define BTDEV_INI MOUNT_POINT"/btdev.ini"

static sdmmc_card_t *card;

static bool SDInit(void)
{
  esp_err_t ret;

  // SPIホストを選択（SPI2 = HSPI, SPI3 = VSPI）
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI2_HOST;   // CoreS3ではSPI2_HOST推奨

  // SPIデバイス（SDカード）の設定
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = GPIO_NUM_4;
  slot_config.host_id = (spi_host_device_t)host.slot;

  // FATFS マウントオプション
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,    // mount時にフォーマットしない
    .max_files = 5,                     // 最大ファイル数
    .allocation_unit_size = 16 * 1024
  };

  // SDカードをマウント
  ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
  if (ret != ESP_OK) {
    m5printf("Failed to mount SD card: %s\n", esp_err_to_name(ret));
    return false;
  }
  m5printf("SD card mounted at " MOUNT_POINT "\n");

  // カード情報表示
  // sdmmc_card_print_info(stdout, card);

  return true;
}

// static char buff[256] = {0};
// const char *FNAME = MOUNT_POINT"/test.txt";
// static char *RWFile(void)
// {
//   FILE *fp = fopen(FNAME, "w");
//   if (!fp) {
//     m5printf("Failed to open file for writing: %s\n", FNAME);
//     return NULL;
//   }
//   fprintf(fp, "Hello, M5stack CoreS3\n");
//   fclose(fp);
    
//   fp = fopen(FNAME, "r");
//   if (!fp) {
//     m5printf("Failed to open file for reading: %s\n", FNAME);
//     return NULL;
//   }
//   fgets(buff, sizeof(buff), fp);
//   fclose(fp);
//   return buff;
// }

static mrb_value
run_mrb_app(mrb_state *mrb, const char *fname)
{
  mrb_value val = mrb_nil_value();

  // open mrb file
  FILE *fp = fopen(fname, "rb");
  if (fp) {
    val = mrb_load_irep_file(mrb, fp);
    fclose(fp);
  }
  
  return val;
}

// static void list_mrb_files(const char *path)
// {
//   DIR *dir = opendir(path);
//   if (dir == NULL) {
//     m5printf("Failed to open directory: %s\n", path);
//     return;
//   }
//   struct dirent *entry;
//   while ((entry = readdir(dir)) != NULL) {
//     if (entry->d_type == DT_REG && strstr(entry->d_name, ".mrb")) {
//       m5printf("Found mrb file: %s/%s\n", path, entry->d_name);
//     }
//   }
//   closedir(dir);
// }

// void app_main(void)
void mrubyTask(void *pvParameters)
{
  // // M5初期化
  // M5.begin();

  printf("Hello, M5stack CoreS3\n");
  M5.update();

  M5.Lcd.fillScreen(BLACK);   // LCD背景を黒に
  M5.Lcd.setTextColor(WHITE); // LCD文字色を白
  // M5.Lcd.setTextSize(2);      // フォントサイズ設定
  M5.Lcd.setTextScroll(true); // スクロール有効化

  // GPIO初期化
  m5gpio_init();

  // // SDカード初期化
  // SDInit();
  // m5printf("SD card initialed.\n");

  // // SDカード内のファイル一覧表示
  // list_mrb_files(MOUNT_POINT);

  // mruby初期化
  m5printf("mruby VM initializing ... ");
  mrb_state *mrb = mrb_open();
  m5printf("%s\n", mrb ? "done" : "failed");

  // // SDカードアクセス確認
  // char *file_content = RWFile();
  // if (file_content) {
  //   m5printf("SD card file content: %s\n", file_content);
  // }
  // else {
  //   m5printf("Failed to read/write SD card file.\n");
  // }

  // // mrubyスクリプト実行
  // mrb_value v = mrb_load_string(mrb, "1+2+3+4+5+6+7+8+9+10");
  // m5printf("mruby return value = %ld\n", mrb_fixnum(v));

  // // 画面中央に文字表示
  // M5.Lcd.setCursor(40, 80);
  // M5.Lcd.print("Hello, CoreS3!");
  
  if (mrb) {
    // SDカード内のautorun.mrbを実行
    mrb_value val = run_mrb_app(mrb, APP_MRB);
    if (mrb->exc) {
      val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
      m5printf("Exception: %s\n", mrb_str_to_cstr(mrb, mrb_obj_as_string(mrb, val)));
      mrb->exc = NULL;
    }
    else {
      if (!mrb_nil_p(val)) {
        m5printf(" => %s\n", mrb_str_to_cstr(mrb, mrb_obj_as_string(mrb, val)));
      }
    }

    // interactive mruby(mirb)を実行
    mirb(mrb);
  }
  
  // メインループ
  m5printf("Start main loop\n");
  M5.update();
  int i=0;
  while (true) {
    m5printf("%i\n", i++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  // mruby終了処理
  mrb_close(mrb);
  printf("mruby VM closed\n");
  M5.update();
}

// 文字列の末尾の改行コードを削除
static void rtrim(char *str)
{
  char *pos = &str[strlen(str) - 1];
  while (pos >= str) {
    if (*pos == '\n' || *pos == '\r') {
      *pos = '\0';
      pos--;
    }
    else {
      break;
    }
  }
}

// BLE初期化
void BLE_init(void)
{
  // BLEデバイス名取得
  FILE *fpbt = fopen(BTDEV_INI, "r");
  if (fpbt) {
    char line[64];
    fgets(line, sizeof(line), fpbt);
    fclose(fpbt);
    // 改行コード削除
    rtrim(line);

    if (strlen(line) > 0) {
      extern void SetBLEDeviceName(const char *name);
      // BLEデバイス名設定
      SetBLEDeviceName(line);
    }
  }

  // BLE GATTサーバー起動
  extern void gatt_app_main(void);
  gatt_app_main();
}

void app_main(void)
{
  // M5初期化
  auto cfg = M5.config();
  M5.begin(cfg);
  
// #if 0 // test
//   M5.Display.setTextSize(2);
//   M5.Display.setCursor(0, 0);
//   M5.Display.println("Waiting for input...");

//   ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
//   esp_vfs_dev_uart_use_driver(UART_NUM_0);

//   char buf[2];
//   while (true) {
//     printf("Type something: ");
//     fflush(stdout);

//     if (fgets(buf, sizeof(buf), stdin)) {
//         M5.Display.printf("Input: %s", buf);
//         printf("Received: %s\n", buf);
//     } else {
//         // printf("Failed to read input\n");
//     }
//     // int len = uart_read_bytes(UART_NUM_0, (uint8_t *)buf, sizeof(buf) - 1, pdMS_TO_TICKS(1000));
//     // if (len > 0) {
//     //     buf[len] = '\0'; // Null-terminate the string
//     //     M5.Display.printf("Input: %s", buf);
//     //     printf("Received: %s\n", buf);
//     // } else {
//     //     // printf("No input received\n");
//     // }
//     M5.update();
//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// #endif

  // LCD初期化
  m5lcd_init();

  // SDカード初期化
  SDInit();
  m5printf("SD card initialed.\n");

  // // BLEデバイス名取得
  // FILE *fpbt = fopen("/sdcard/btdev.ini", "r");
  // if (fpbt) {
  //   char line[128];
  //   fgets(line, sizeof(line), fpbt);
  //   fclose(fpbt);
  //   // 改行コード削除
  //   for (char *pos = &line[strlen(line) - 1]; pos >= line; pos--) {
  //     if (*pos == '\n' || *pos == '\r') *pos = '\0';
  //     else break;
  //   }
  //   if (strlen(line) > 0) {
  //     extern void SetBLEDeviceName(const char *name);
  //     SetBLEDeviceName(line);
  //   }
  // }

  // // BLE GATTサーバー起動
  // extern void gatt_app_main(void);
  // gatt_app_main();

  // BLE初期化
  BLE_init();

  // mrubyタスク起動
  xTaskCreate(mrubyTask, "mrubyTask", 16384, NULL, 5, NULL);

  while (true) {
    M5.update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

}
