/*
 * A355467  a(n) is the smallest number which is greater than n and has more
 *          prime factors (with multiplicity) than n.
 * https://oeis.org/A355467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: smallest integer m > n with more prime factors
 * (counted with multiplicity) than n (i.e. bigomega(m) > bigomega(n)). */
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t target = bigomega(n);   /* bigomega(n) fits in seq_t */

    idx_t m = n;                  /* will be incremented to > n */
    for (;;) {
        ++m;                      /* ensure m > n */
        if (bigomega(m) > target)
            break;
    }
    return (seq_t)m;
}
