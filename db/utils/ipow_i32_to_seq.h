/* ipow_i32_to_seq: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_I32_TO_SEQ_H
#define OEISC_UTILS_IPOW_I32_TO_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--------- small‑base exponent (0‑9 ^ 0‑9) --------*/
static inline seq_t ipow_i32_to_seq(int base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

#endif
