#pragma once

#include <stdio.h>

#ifdef DEBUG
#define debug_printf(fmt, ...) \
  {                           \
    fprintf(stdout, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__); \
  }
#define debug_print(fmt) debug_printf(fmt, "")
#else
#define debug_printf(fmt, ...)
#define debug_print(fmt)
#endif

