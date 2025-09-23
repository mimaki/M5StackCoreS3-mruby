#include "mruby.h"
#include "mruby/variable.h"
// #include "mruby/class.h"
#include "mruby/string.h"
// #include "mruby/array.h"
// #include "mruby/data.h"
// #include <string.h>

#define BLE_ID_MIN  0
#define BLE_ID_MAX  1

// BLE.new(id=0) #=> self
static mrb_value
mrb_ble_init(mrb_state *mrb, mrb_value self)
{
  mrb_int id = 0;
  mrb_get_args(mrb, "|i", &id);
  if (id < BLE_ID_MIN || id > BLE_ID_MAX) {
    id = BLE_ID_MIN;
  }
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "id"), mrb_fixnum_value(id));
  return self;
}

void GattNotify(uint16_t, uint8_t*, size_t);
uint8_t *GetGattWriteData(uint16_t);
uint8_t IsBLEConnected(uint16_t);

// BLE#notify(data) #=> self
static mrb_value
mrb_ble_notify(mrb_state *mrb, mrb_value self)
{
  char *s;
  mrb_int len;
  mrb_int id = 0;
  mrb_get_args(mrb, "s", &s, &len);
  GattNotify(id, (uint8_t*)s, (size_t)len);
  return self;
}

// BLE#read #=> String
static mrb_value
mrb_ble_read(mrb_state *mrb, mrb_value self)
{
  mrb_int id = 0;
//   return mrb_str_new_cstr(mrb, (char*)GetGattWriteData(id));
  return mrb_str_new_cstr(mrb, "");  // dummy
}

// BLE#connected? #=> true/false
static mrb_value
mrb_ble_is_conn(mrb_state *mrb, mrb_value self)
{
  mrb_int id = 0;
  // uint8_t isConn = IsBLEConnected(id);
  // return mrb_bool_value((mrb_bool)isConn);
  return mrb_true_value();  // dummy
}

void
mrb_mruby_m5stack_ble_gatt_gem_init(mrb_state *mrb)
{
  struct RClass *ble  = mrb_define_class(mrb, "BLE", mrb->object_class);

  mrb_define_method(mrb, ble, "initialize", mrb_ble_init,     MRB_ARGS_OPT(1));
  mrb_define_method(mrb, ble, "notify",     mrb_ble_notify,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ble, "read",       mrb_ble_read,     MRB_ARGS_NONE());
  mrb_define_method(mrb, ble, "connected?", mrb_ble_is_conn,  MRB_ARGS_NONE());
}

void
mrb_mruby_m5stack_ble_gatt_gem_final(mrb_state *mrb)
{
}
