/* triangular_seq: n(n+1)/2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TRIANGULAR_SEQ_H
#define OEISC_UTILS_TRIANGULAR_SEQ_H
#include <stdint.h>
#include <stdlib.h>

/* Compute the next iterated triangular number.
 * The division by 2 is performed before multiplication to avoid
 * overflow on built‑in unsigned types.  The same logic works for
 * arbitrary‑precision classes. */
static inline seq_t triangular_seq(seq_t x)
{
    if ((x & (seq_t)1) != 0) {          /* x is odd, (x+1) even */
        return x * ((x + (seq_t)1) >> 1);
    } else {
        return (x >> 1) * (x + (seq_t)1);
    }
}

#endif
