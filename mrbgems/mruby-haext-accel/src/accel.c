#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/data.h"
#include <string.h>
#include <M5CUtil.h>

// C function to be called from mruby
static mrb_value c_hello(mrb_state *mrb, mrb_value self) {
    mrb_value ary = mrb_ary_new(mrb);
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 1.1));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 2.2));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 3.3));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 4.4));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 5.5));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, 6.6));
    return ary;
}

static mrb_value c_acc(mrb_state *mrb, mrb_value self) {    
    float x, y, z;
    m5accel_read(&x, &y, &z);
    mrb_value ary = mrb_ary_new(mrb);
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, x));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, y));
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, z));
    return ary;
}

// Initialize and bind the C function to mruby
void mrb_mruby_haext_accel_gem_init(mrb_state* mrb) {
    struct RClass *mod = mrb_define_module(mrb, "HAExt");
    mrb_define_class_method(mrb, mod, "hello", c_hello, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mod, "acc", c_acc, MRB_ARGS_NONE());
}

void mrb_mruby_haext_accel_gem_final(mrb_state* mrb) {
    // cleanup if needed
}
