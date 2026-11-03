/*
 * A368573  a(n) = Sum_{k=0..n} k! * k^k.
 * https://oeis.org/A368573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k! * k^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum  = 0;     /* running total of the series */
    seq_t fact = 1;     /* current factorial k! (0! = 1) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute k^k, treating 0^0 as 1 */
        seq_t pow_k = ipow((seq_t)k, k);

        /* term = k! * k^k */
        seq_t term = seq_mul(fact, pow_k);

        sum += term;

        /* update factorial for next k, unless we are at the last k */
        if (k != n)
            fact = seq_mul(fact, (seq_t)(k + 1));
    }
    return sum;
}
