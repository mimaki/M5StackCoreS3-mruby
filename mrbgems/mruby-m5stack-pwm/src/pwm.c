#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
// #include <stdbool.h>
#include <M5CUtil.h>

static mrb_value
mrb_pwm_init(mrb_state *mrb, mrb_value self)
{
  mrb_int pin = 0;  // dummy

  mrb_get_args(mrb, "|i", &pin);

  return self;
}

static mrb_value
mrb_pwm_duty(mrb_state *mrb, mrb_value self)
{
  mrb_int v;

  mrb_get_args(mrb, "i", &v);

#ifndef NO_ESP32
  m5pwm_write((uint32_t)v);
#endif /* NO_ESP32 */

  return mrb_fixnum_value(v);
}

static mrb_value
mrb_pwm_freq(mrb_state *mrb, mrb_value self)
{
  mrb_int v;

  mrb_get_args(mrb, "i", &v);

#ifndef NO_ESP32
  m5pwm_set_freq((uint32_t)v);
#endif /* NO_ESP32 */

  return mrb_fixnum_value(v);
}

void
mrb_mruby_m5stack_pwm_gem_init(mrb_state *mrb)
{
  struct RClass *pwm = mrb_define_class(mrb, "PWM", mrb->object_class);

  mrb_define_method(mrb, pwm, "initialize", mrb_pwm_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, pwm, "frequency",  mrb_pwm_freq, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, pwm, "duty",       mrb_pwm_duty, MRB_ARGS_REQ(1));
}

void
mrb_mruby_m5stack_pwm_gem_final(mrb_state *mrb)
{
}
