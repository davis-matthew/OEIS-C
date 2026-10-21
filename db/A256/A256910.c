/*
 * A256910  Trace of the enhanced triangular-number representation of n.
 * https://oeis.org/A256910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(2)=2. For n>0, let T be the largest triangular number
 *            <= n. If T==n then a(n)=n, otherwise a(n)=a(n-T). (The sequence
 *            is the trace of the enhanced triangular-number representation.)
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_triangular_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_triangular_leq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    for (;;) {
        if (n == 2) return 2;      /* special base case */
        idx_t t = largest_triangular_leq(n);
        if (t == n)               /* n itself is triangular */
            return (seq_t)n;
        n -= t;                    /* recurse on the remainder */
    }
}
