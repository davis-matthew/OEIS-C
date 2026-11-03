/* pow3_seq: 3^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW3_SEQ_H
#define OEISC_UTILS_POW3_SEQ_H
#include <stdbool.h>
#include <stdint.h>

/* Compute 3^exp where exp fits in seq_t.  Binary exponentiation. */
static seq_t pow3_seq(seq_t exp)
{
    seq_t result = 1;
    seq_t base = 3;

    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

#endif
