/*
 * A343900  a(n) = Sum_{k=0..n} (k!)^(k+1) * binomial(n,k).
 * https://oeis.org/A343900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^(k+1) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 29 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;           /* fact = k! for current k (k starts at 0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (k!)^(k+1)  */
        seq_t pow_k = ipow_idx(fact, k + 1);

        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* term = (k!)^(k+1) * C(n,k) */
        seq_t term = seq_mul(pow_k, bin);

        /* accumulate towards the sum */
        sum += term;

        /* update factorial for next iteration: (k+1)! = k! * (k+1) */
        if (k != n)
            fact = seq_mul(fact, (seq_t)(k + 1));
    }

    return sum;
}
