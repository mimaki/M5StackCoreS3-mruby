/*
 ** mrb.c - MRB class
 */

#include "mruby.h"
// #include "mruby/error.h"
#include "mruby/array.h"
// #include "mruby/class.h"
// #include "mruby/numeric.h"
// #include "mruby/string.h"
// #include "mruby/variable.h"

// #include <ctype.h>
// #include <errno.h>
// #include <limits.h>
#include <stdio.h>
#include <string.h>

#include "M5CUtil.h"
#include "esp_system.h"

// struct tag_mrb_entry {
//   struct tag_mrb_entry *next;
//   char *name;
// };
// typedef struct tag_mrb_entry mrb_entry;

#define MRB_DIR "/sdcard/M5MRB/"

static mrb_value
mrb_mrb_list(mrb_state *mrb, mrb_value self)
{
  mrb_value ary = mrb_ary_new(mrb);
  mrb_int ai = mrb_gc_arena_save(mrb);
  mrb_value val;

  // Get file list in `/M5MRB`
  dir_entry *topmrb = list_mrb_files(MRB_DIR);
  for (dir_entry *app=topmrb; app->next; app=app->next) {
    char *name = app->name;
    // m5printf("app->name: %s\n", name);
    if (strlen(name) <= 4) continue;
    name[strlen(name) - 4] = '\0'; // remove .mrb extension
    // m5printf("name: %s\n", name);
    val = mrb_str_new_cstr(mrb, name);
    mrb_ary_push(mrb, ary, val);
  }
  // val = mrb_str_new_cstr(mrb, "foo"); mrb_ary_push(mrb, ary, val);
  // val = mrb_str_new_cstr(mrb, "bar"); mrb_ary_push(mrb, ary, val);
  // val = mrb_str_new_cstr(mrb, "baz"); mrb_ary_push(mrb, ary, val);
  // val = mrb_str_new_cstr(mrb, "12345678"); mrb_ary_push(mrb, ary, val);
  // val = mrb_str_new_cstr(mrb, "abcdefgh"); mrb_ary_push(mrb, ary, val);
  // val = mrb_str_new_cstr(mrb, "hello"); mrb_ary_push(mrb, ary, val);

  // Clean up file list
  free_dir_list(topmrb);

  mrb_gc_arena_restore(mrb, ai);

  return ary;
}

#define ONESHOT_NAME "/sdcard/oneshot.ini"

static mrb_value
mrb_mrb_run(mrb_state *mrb, mrb_value self)
{
  char *str = NULL;
  mrb_get_args(mrb, "z", &str);
  // printf("mrb: %s\n", str);

  // Write mrb name into `/oneshot.ini` and reset M5Stack
  FILE *fp = fopen(ONESHOT_NAME, "w");
  if (!fp) {
    m5printf("Cannot run %s.\n", str);
    return mrb_nil_value();
  }
  fprintf(fp, MRB_DIR"%s.mrb\n", str);
  fclose(fp);
  m5delay(100);

  // Reset M5Stack
  esp_restart();
  return mrb_nil_value();
}

void
mrb_mruby_m5stack_mrb_gem_init(mrb_state *mrb)
{
  struct RClass *clsmrb = mrb_define_class(mrb, "MRB", mrb->object_class);

  mrb_define_class_method(mrb, clsmrb, "list", mrb_mrb_list, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, clsmrb, "run", mrb_mrb_run, MRB_ARGS_REQ(1));
}

void
mrb_mruby_m5stack_mrb_gem_final(mrb_state *mrb)
{
}
