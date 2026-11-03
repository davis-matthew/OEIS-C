/*
 * A394786  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x,y,z are positive integers satisfying x^2 < y*z.
 * https://oeis.org/A394786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{x≥1, x² < n−x²} τ(n − x²) where τ(m) is the number of
 *            positive divisors of m
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-01)
 * Helpers:   num_divisors_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the direct definition of OEIS A394786. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/num_divisors_idx.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t x = 1; ; ++x) {
        idx_t x_sq = x * x;
        if (x_sq >= n)         /* x² ≥ n ⇒ no further solutions */
            break;
        idx_t t = n - x_sq;    /* t = y·z */
        if (x_sq < t) {        /* enforce x² < y·z */
            result += num_divisors_idx(t);
        }
    }
    return result;
}
