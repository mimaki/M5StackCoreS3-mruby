#include <stdio.h>
#include "mruby.h"
#include "mruby/class.h"
#include <M5CUtil.h>

static mrb_value
mrb_stdout_putc(mrb_state *mrb, mrb_value self)
{
  mrb_int c;
  mrb_get_args(mrb, "i", &c);
  // putchar((char)c);
  // fflush(stdout);
  m5putchar((char)c);
  return mrb_nil_value();
}

static mrb_value
mrb_stderr_putc(mrb_state *mrb, mrb_value self)
{
  mrb_int c;
  mrb_get_args(mrb, "i", &c);
  // putchar((char)c);
  // fflush(stderr);
  m5puterrchar((char)c);
  return mrb_nil_value();
}

static mrb_value
mrb_stdin_getc(mrb_state *mrb, mrb_value self)
{
  mrb_int c = fgetc(stdin);  
  return mrb_fixnum_value(c);
}

void
mrb_mruby_stdio_m5stack_gem_init(mrb_state *mrb)
{
  struct RClass *cin  = mrb_define_class(mrb, "STDIN",  mrb->object_class);
  struct RClass *cout = mrb_define_class(mrb, "STDOUT", mrb->object_class);
  struct RClass *cerr = mrb_define_class(mrb, "STDERR", cout);

  mrb_define_class_method(mrb, cin,   "_getc", mrb_stdin_getc,  MRB_ARGS_NONE());
  mrb_define_class_method(mrb, cout,  "_putc", mrb_stdout_putc, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, cerr,  "_err_putc", mrb_stderr_putc, MRB_ARGS_REQ(1));
}

void
mrb_mruby_stdio_m5stack_gem_final(mrb_state *mrb)
{
}
