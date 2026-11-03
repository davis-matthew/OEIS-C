/* ipow_idx_to_seq: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_IDX_TO_SEQ_H
#define OEISC_UTILS_IPOW_IDX_TO_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Power of a prime: base^exp (exp small enough to fit in seq_t) */
static seq_t ipow_idx_to_seq(idx_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t b = (seq_t)base;
    seq_t e = (seq_t)exp;
    /* repeated squaring */
    while (e > 0) {
        if (e & 1) res = res * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return res;
}

#endif
