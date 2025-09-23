#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
// #include <stdbool.h>
#include <M5CUtil.h>

static mrb_value
mrb_gpio_init(mrb_state *mrb, mrb_value self)
{
  mrb_int pin;
  mrb_int mode = 0;

  mrb_get_args(mrb, "i|i", &pin, &mode);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pin"), mrb_fixnum_value(pin));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@mode"), mrb_fixnum_value(mode));

  return self;
}

// static mrb_value
// mrb_gpio_read(mrb_state *mrb, mrb_value self)
// {
//   mrb_value vpin/*, vout*/;
//   mrb_int v = 0;

//   vpin = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pin"));
// //   vout = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@out"));

// #ifndef NO_ESP32
// //   if (mrb_nil_p(vout) || mrb_bool(vout)) {
// //     /* configure pin to digital-in */
// //     pinMode(mrb_fixnum(vpin), INPUT);
// //     mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@out"), mrb_false_value());
// //   }
// //   v = digitalRead(mrb_fixnum(vpin));
//     // v = m5gpio_read(mrb_fixnum(vpin));
// #endif /* NO_ESP32 */

//   return mrb_fixnum_value(v);
// }

static mrb_value
mrb_gpio_write(mrb_state *mrb, mrb_value self)
{
  mrb_value vpin/*, vout*/;
  mrb_int v;

  mrb_get_args(mrb, "i", &v);

  vpin = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pin"));
//   vout = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@out"));

#ifndef NO_ESP32
//   if (mrb_nil_p(vout) || !mrb_bool(vout)) {
//     /* configure pin to digital-out */
//     pinMode(mrb_fixnum(vpin), OUTPUT);
//     mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@out"), mrb_true_value());
//   }
//   digitalWrite(mrb_fixnum(vpin), (uint8_t)v);
    m5gpio_write(mrb_fixnum(vpin), (int)v);
#endif /* NO_ESP32 */

  return mrb_fixnum_value(v);
}

void
mrb_mruby_m5stack_gpio_gem_init(mrb_state *mrb)
{
  struct RClass *gpio   = mrb_define_class(mrb, "GPIO", mrb->object_class);

  mrb_define_method(mrb, gpio, "initialize", mrb_gpio_init,   MRB_ARGS_ARG(1, 1));
//   mrb_define_method(mrb, dio, "read",       mrb_gpio_read,   MRB_ARGS_NONE());
  mrb_define_method(mrb, gpio, "write",      mrb_gpio_write,  MRB_ARGS_REQ(1));
}

void
mrb_mruby_m5stack_gpio_gem_final(mrb_state *mrb)
{
}
