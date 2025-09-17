#include <M5CUtil.h>

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/variable.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

static mrb_value
mrb_sleep(mrb_state *mrb, mrb_value self)
{
  mrb_value t;
  uint32_t sec;
  mrb_get_args(mrb, "o", &t);
  if (mrb_fixnum_p(t)) {
    sec = (uint32_t)mrb_fixnum(t) * 1000;
  }
  else if (mrb_float_p(t)) {
    sec = (uint32_t)(mrb_float(t) * 1000.0);
  }
  else {
    mrb_raise(mrb, E_TYPE_ERROR, "can't convert time interval");
  }
  m5delay(sec);
  return mrb_nil_value();
}

static mrb_value
mrb_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int t;
  uint32_t ms;
  mrb_get_args(mrb, "i", &t);
  ms = (uint32_t)t;
  m5delay(ms);
  return mrb_nil_value();
}

// static mrb_value
// mrb_delay_us(mrb_state *mrb, mrb_value self)
// {
//   mrb_int t;
//   mrb_get_args(mrb, "i", &t);
//   ets_delay_us((uint32_t)t);
//   return mrb_nil_value();
// }

void
mrb_mruby_sleep_esp32_gem_init(mrb_state *mrb)
{
  // module functions
  mrb_define_method(mrb, mrb->kernel_module, "sleep",     mrb_sleep,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "delay",     mrb_delay,    MRB_ARGS_REQ(1));
  // mrb_define_method(mrb, mrb->kernel_module, "delay_us",  mrb_delay_us, MRB_ARGS_REQ(1));
}

void
mrb_mruby_sleep_esp32_gem_final(mrb_state *mrb)
{
}

// #ifdef __cplusplus
// }
// #endif
