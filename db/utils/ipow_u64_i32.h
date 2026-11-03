/* ipow_u64_i32: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_I32_H
#define OEISC_UTILS_IPOW_U64_I32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------------------- */
static seq_t ipow_u64_i32(uint64_t base, int exp)
{
    seq_t r = 1;
    seq_t b = (seq_t)base;
    for (int i = 0; i < exp; ++i)
        r *= b;
    return r;
}

#endif
