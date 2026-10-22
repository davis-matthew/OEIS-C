/*
 * A369300  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 * (1-x^3)^3 ).
 * https://oeis.org/A369300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(3*n+k+2, k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binom, binom_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/binom_exact.h"

seq_t a(idx_t n)
{
    /* For 64‑bit builds, compute exactly only for n where all
       intermediate binomials fit in 64 bits.  Empirically that is
       n ≤ 25; for larger n we fall back to the generic binom() helper
       which may overflow – this is acceptable because the
       corresponding a(n) no longer fits in 64 bits. */
    const idx_t MAX_SMALL = 25;

    idx_t limit = n / 3;

    if (sizeof(seq_t) <= 8 && n <= MAX_SMALL)
    {
        __uint128_t sum = 0;
        for (idx_t k = 0; k <= limit; ++k)
        {
            seq_t b1 = binom_exact(3 * n + k + 2, k);
            seq_t b2 = binom_exact(3 * n - 3 * k + 1, n - 3 * k);
            sum += (__uint128_t)b1 * (__uint128_t)b2;
        }
        return (seq_t)(sum / (__uint128_t)(n + 1));
    }
    else
    {
        seq_t sum = 0;
        for (idx_t k = 0; k <= limit; ++k)
        {
            seq_t b1 = binom(3 * n + k + 2, k);
            seq_t b2 = binom(3 * n - 3 * k + 1, n - 3 * k);
            sum += b1 * b2;
        }
        return sum / ((seq_t)(n + 1));
    }
}
