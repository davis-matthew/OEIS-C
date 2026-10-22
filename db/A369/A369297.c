/*
 * A369297  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 * (1-x^3) ).
 * https://oeis.org/A369297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* multiplicative binomial using gcd to keep intermediates small.
 * Works for any seq_t (fixed‑size or arbitrary‑precision). */
static seq_t binom_multiplicative(idx_t top, idx_t bottom)
{
    if (bottom > top) return 0;
    if (bottom == 0 || bottom == top) return 1;

    if (bottom > top - bottom) bottom = top - bottom;   /* symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= bottom; ++i)
    {
        seq_t mult = (seq_t)(top - bottom + i);
        seq_t den  = (seq_t)i;

        /* cancel common factors between result and denominator */
        seq_t g1 = gcd(result, den);
        result /= g1;
        den    /= g1;

        /* cancel remaining factors between numerator and denominator */
        seq_t g2 = gcd(mult, den);
        mult  /= g2;
        den   /= g2;          /* den should be 1 now */

        result = result * mult; /* exact integer */
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum  = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t top1    = n + k;
        idx_t bottom1 = k;
        seq_t part1   = binom_multiplicative(top1, bottom1);

        idx_t top2    = 3 * n - 3 * k + 1;
        idx_t bottom2 = n - 3 * k;
        seq_t part2   = binom_multiplicative(top2, bottom2);

        sum += part1 * part2;
    }

    seq_t denom = (seq_t)n + 1;
    return sum / denom;   /* division is exact by definition */
}
