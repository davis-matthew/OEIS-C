/* ctz_idx_to_i32: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_IDX_TO_I32_H
#define OEISC_UTILS_CTZ_IDX_TO_I32_H
#include <stdint.h>
#include <stdlib.h>

static inline int ctz_idx_to_i32(idx_t v) { return __builtin_ctzll((unsigned long long)v); }

#endif
