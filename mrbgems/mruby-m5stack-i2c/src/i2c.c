#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/data.h"
#include <string.h>
#include "M5CUtil.h"

// #ifndef NO_DEVICE
//   #include "Wire.h"
// #endif

// #ifdef __cplusplus
// extern "C" {
// #endif

// typedef struct _mrb_esp32_i2c {
//   void *i2c;    /* I2C object */
//   mrb_int addr; /* I2C slave address */
// } mrb_esp32_i2c;

// #ifndef NO_DEVICE
// #define WIRE(ptr) ((TwoWire*)(ptr->i2c))
// #endif

// static void
// mrb_esp32_i2c_free(mrb_state *mrb, void *ptr)
// {
//   mrb_esp32_i2c *i2c = (mrb_esp32_i2c*)ptr;
// #ifndef NO_DEVICE
//   if (i2c->i2c) {
//     i2c->i2c = NULL;
//   }
// #endif
//   mrb_free(mrb, ptr);
// }

// static const struct mrb_data_type mrb_i2c_type = { "mbedI2C", mrb_esp32_i2c_free };

// I2C#new(id=0, params={}) #=> I2C object
static mrb_value
mrb_i2c_init(mrb_state *mrb, mrb_value self)
{
  mrb_int id = 0;
  mrb_value params = mrb_nil_value();
  // mrb_esp32_i2c *i2c;

  mrb_get_args(mrb, "|io", &id, &params);

  // m5printf("I2C#new(%d)\n", (int)id);

  // i2c = (mrb_esp32_i2c*)mrb_malloc(mrb, sizeof(mrb_esp32_i2c));
  /* initialize I2C object */
#ifdef NO_DEVICE
  // i2c->i2c = NULL;
#else /* ESP32 */
  // i2c->i2c = (void*)1; // dummy
  m5i2c_init();
#endif
  // i2c->addr = 0;
  // mrb_data_init(self, i2c, &mrb_i2c_type);
  return self;
}

// I2C#read(len) #=> String
static mrb_value
mrb_i2c_read(mrb_state *mrb, mrb_value self)
{
  mrb_int addr, len, i;
  mrb_value v, params = mrb_ary_new(mrb);
  uint8_t *buf;
  // mrb_esp32_i2c *i2c = (mrb_esp32_i2c*)DATA_PTR(self);

// #ifndef NO_DEVICE
//   if (!i2c->i2c) mrb_raise(mrb, E_RUNTIME_ERROR, "I2C device is already closed.");
// #endif

  mrb_get_args(mrb, "ii|A", &addr, &len, &params);
  // m5printf("I2C#read(addr=0x%02x, len=%d, params)\n", (int)addr, (int)len);

#if 1
#ifndef NO_DEVICE
  if (mrb_array_p(params)) {
    mrb_int arylen = RARRAY_LEN(params);
    if (arylen > 0) {
      // WIRE(i2c)->beginTransmission((int)addr);
      // for (i=0; i<arylen; i++) {
      //   WIRE(i2c)->write((uint8_t)mrb_fixnum(mrb_ary_ref(mrb, params, i)));
      // }
      // WIRE(i2c)->endTransmission((uint8_t)0);
      uint8_t *buf = (uint8_t*)mrb_malloc(mrb, arylen);
if (arylen > 0) m5printf("  param:");
      for (i=0; i<arylen; i++) {
        buf[i] = (uint8_t)mrb_fixnum(mrb_ary_ref(mrb, params, i));
        // m5printf(" %02x ", buf[i]);
      }
if (arylen > 0) m5printf("\n");
      m5i2c_write((uint8_t)addr, buf, arylen, 0);
      mrb_free(mrb, buf);
    }
  }
#endif

  buf = (uint8_t*)mrb_malloc(mrb, len);
  memset(buf, 0, len);

#ifndef NO_DEVICE
  // WIRE(i2c)->requestFrom((int)addr, len);
  // for (i=0; i<len; i++) {
  //   buf[i] = (uint8_t)WIRE(i2c)->read();
  // }
  size_t rlen = m5i2c_read((uint8_t)addr, buf, len, 1);
  if (rlen != (size_t)len) {
    // mrb_raise(mrb, E_RUNTIME_ERROR, "I2C read error.");
    len = (mrb_int)rlen;
  }
#endif
  v = mrb_str_new(mrb, (const char*)buf, len);
  // if (len > 0) {
  //   m5printf("  read:");
  //   for (i=0; i<len; i++) {
  //     m5printf("%02x ", buf[i]);
  //   }
  //   m5printf("\n");
  // }
  mrb_free(mrb, buf);

#else

  mrb_int cmdlen = 0;
#ifndef NO_DEVICE
  if (mrb_array_p(params)) {
    cmdlen = RARRAY_LEN(params);
    if (cmdlen > 0) {
      uint8_t *cmd = (uint8_t*)mrb_malloc(mrb, cmdlen);
if (cmdlen > 0) m5printf("  param:");
      for (i=0; i<cmdlen; i++) {
        cmd[i] = (uint8_t)mrb_fixnum(mrb_ary_ref(mrb, params, i));
m5printf(" %02x ", cmd[i]);
      }
if (cmdlen > 0) m5printf("\n");
    }
  }

  buf = (uint8_t*)mrb_malloc(mrb, len);
  memset(buf, 0, len);

  // WIRE(i2c)->requestFrom((int)addr, len);
  // for (i=0; i<len; i++) {
  //   buf[i] = (uint8_t)WIRE(i2c)->read();
  // }
  // size_t rlen = m5i2c_read((uint8_t)addr, buf, len, 1);

//  bool readRegister(std::uint8_t address, std::uint8_t reg, std::uint8_t* result, std::size_t length, std::uint32_t freq) const;
  size_t rlen = m5i2c_regread((uint8_t)addr, cmd, arylen);
  if (rlen != (size_t)len) {
    // mrb_raise(mrb, E_RUNTIME_ERROR, "I2C read error.");
    len = (mrb_int)rlen;
  }
#endif
  v = mrb_str_new(mrb, (const char*)buf, len);
if (len > 0) {
  m5printf("  read:");
  for (i=0; i<len; i++) {
    m5printf("%02x ", buf[i]);
  }
  m5printf("\n");
}
  mrb_free(mrb, buf);

#endif

  return v;
}

// I2C#write(data, stop=true) #=> true/false
static mrb_value
mrb_i2c_write(mrb_state *mrb, mrb_value self)
{
  mrb_value data, v;
  mrb_int addr;
  uint8_t *buf;
  size_t len, i;
  mrb_bool stop = 1;
  uint8_t ret = 0;
//   // mrb_esp32_i2c *i2c = (mrb_esp32_i2c*)DATA_PTR(self);

// // #ifndef NO_DEVICE
// //   if (!i2c->i2c) mrb_raise(mrb, E_RUNTIME_ERROR, "I2C device is already closed.");
// // #endif

  mrb_get_args(mrb, "io|b", &addr, &data, &stop);
  // m5printf("I2C#write(addr=0x%02x, data, stop=%d)\n", (int)addr, stop ? 1 : 0);

  if (mrb_string_p(data)) {
    // m5printf("  write string data\n");
    len = RSTRING_LEN(data);
    buf = (uint8_t*)mrb_malloc(mrb, len);
    memcpy(buf, RSTRING_PTR(data), len);
  }
  else if (mrb_array_p(data)) {
    // m5printf("  write array data\n");
    len = RARRAY_LEN(data);
    buf = (uint8_t*)mrb_malloc(mrb, len);
    for (i=0; i<len; i++) {
      buf[i] = (uint8_t)mrb_fixnum(mrb_ary_ref(mrb, data, i));
    }
  }
  else if (mrb_fixnum_p(data)) {
    // m5printf("  write integer data\n");
    len = 1;
    buf = (uint8_t*)mrb_malloc(mrb, len);
    buf[0] = (uint8_t)mrb_fixnum(data);
  }
  else {
    // m5printf("  write object data\n");
    v = mrb_obj_as_string(mrb, data);
    len = RSTRING_LEN(v);
    buf = (uint8_t*)mrb_malloc(mrb, len);
    memcpy(buf, RSTRING_PTR(v), len);
  }

// if (len > 0) {
//   m5printf("  write:");
//   for (i=0; i<len; i++) {
//     m5printf("%02x ", buf[i]);
//   }
//   m5printf("\n");
// }

#ifndef NO_DEVICE
//   WIRE(i2c)->beginTransmission((int)addr);
//   /* write data to I2C */
//   for (i=0; i<len; i++) {
//     WIRE(i2c)->write(buf[i]);
//   }
//   ret = WIRE(i2c)->endTransmission((uint8_t)(stop ? 1 : 0));
  ret = m5i2c_write((uint8_t)addr, buf, len, (uint8_t)(stop ? 1 : 0));
#endif

  mrb_free(mrb, buf);

  return mrb_bool_value((mrb_bool)(ret == 0));
}

void
mrb_mruby_m5stack_i2c_gem_init(mrb_state *mrb)
{
  struct RClass *i2c  = mrb_define_class_under(mrb, mrb->object_class, "I2C", mrb->object_class);
  // MRB_SET_INSTANCE_TT(i2c, MRB_TT_DATA);

//   Wire.begin();

  mrb_define_method(mrb, i2c, "initialize", mrb_i2c_init,     MRB_ARGS_OPT(2));
  mrb_define_method(mrb, i2c, "read",       mrb_i2c_read,     MRB_ARGS_REQ(2)|MRB_ARGS_ANY());
  mrb_define_method(mrb, i2c, "write",      mrb_i2c_write,    MRB_ARGS_ARG(2, 1));
}

void
mrb_mruby_m5stack_i2c_gem_final(mrb_state *mrb)
{
}

// #ifdef __cplusplus
// }
// #endif
