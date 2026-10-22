/*
 * A366100  Expansion of (1/x) * Series_Reversion( x*(1+x-x^4)/(1+x)^4 ).
 * https://oeis.org/A366100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (1/(n+1)) * Σ_{k=0..⌊n/4⌋} binomial(n+k, k) * binomial(3*n-k+3, n-4*k)
 * This function uses the provided binomial implementation from utils.
 */
#include <stddef.h>                     /* for size_t */
#include "../utils/binomial.h"           /* binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* pre‑compute the largest k */
    const idx_t maxk = n / 4;

    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(n+k, k) */
        seq_t term1 = binomial(n + k, k);

        /* binomial(3*n - k + 3, n - 4*k) */
        idx_t a = 3 * n - k + 3;
        idx_t b = n - 4 * k;
        seq_t term2 = binomial(a, b);

        /* Add t1 * t2 to the running sum */
        sum += term1 * term2;
    }

    /* Final division by (n+1) */
    return sum / (seq_t)(n + 1);
}
