/* pow2_i32: 2^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW2_I32_H
#define OEISC_UTILS_POW2_I32_H
#include <stdbool.h>
#include <stdint.h>

/* Compute 2^e as a seq_t value (iterative multiplication). */
static seq_t pow2_i32(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i)
        r *= 2;
    return r;
}

#endif
