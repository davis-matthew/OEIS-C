/* popcount_i32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_I32_H
#define OEISC_UTILS_POPCOUNT_I32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int popcount_i32(int m) { return __builtin_popcount((unsigned)m); }

#endif
