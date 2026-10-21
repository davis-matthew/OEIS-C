/*
 * A278291  Numbers n such that n-1 has the same number of prime factors as n
 *          (with multiplicity).
 * https://oeis.org/A278291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 9139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278291: numbers n such that n-1 has the same
   number of prime factors (with multiplicity) as n.
   The function implements a linear search for the nth such number. */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    idx_t found = 0;
    for (idx_t m = 2; ; ++m) {    /* 3 is the first number satisfying the condition */
        if (bigomega(m) == bigomega(m - 1)) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
    }
}
