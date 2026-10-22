/*
 * A321386  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^(d-1).
 * https://oeis.org/A321386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^(d-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Positive and negative contributions are accumulated separately
       to keep everything unsigned.  The final result is guaranteed
       non‑negative. */
    seq_t pos = 0;
    seq_t neg = 0;

    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i != 0) {
            continue;           /* not a divisor */
        }

        /* first divisor d = i */
        idx_t d  = i;
        idx_t q  = n / d;       /* n/d for the sign test */
        seq_t term_d = seq_pow((seq_t)d, d - 1U);

        if (q & 1U) {           /* q odd -> (+1) */
            pos += term_d;
        } else {                /* q even -> (-1) */
            neg += term_d;
        }

        /* complementary divisor e = n / i, unless it equals d */
        idx_t e = n / i;
        if (e == d) {
            continue;           /* already processed */
        }

        idx_t q2 = n / e;       /* which is i */
        seq_t term_e = seq_pow((seq_t)e, e - 1U);

        if (q2 & 1U) {           /* i odd -> (+1) */
            pos += term_e;
        } else {                /* i even -> (-1) */
            neg += term_e;
        }
    }

    return pos - neg;
}
