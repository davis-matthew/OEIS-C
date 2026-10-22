/*
 * A346680  a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(4*k,k) / (3*k + 1).
 * https://oeis.org/A346680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346680 (OEIS)
   a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(4k,k) / (3k+1)
   This implementation uses the binomial‐term recurrence
   to avoid large intermediate values for 64‑bit builds
   and full arbitrary‑precision calculations for other builds.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* fast path for 64‑bit seq_t: use 128‑bit intermediates */
    if (sizeof(seq_t) <= 8)
    {
        __uint128_t binom = 1;          /* B_k = binomial(4k,k)   */
        __uint128_t pos   = 0;          /* sum of terms with even sign */
        __uint128_t neg   = 0;          /* sum of terms with odd  sign */

        for (idx_t k = 0; k <= n; ++k)
        {
            __uint128_t tk = binom / (3 * k + 1);

            if ((n - k) & 1u)
                neg += tk;
            else
                pos += tk;

            if (k == n) break;

            /* update binom to next value */
            __uint128_t c1 = 4 * k + 4;
            __uint128_t c2 = 4 * k + 3;
            __uint128_t c3 = 4 * k + 2;
            __uint128_t c4 = 4 * k + 1;

            binom *= c1; binom *= c2; binom *= c3; binom *= c4;

            __uint128_t d1 = k + 1;
            __uint128_t d2 = 3 * k + 3;
            __uint128_t d3 = 3 * k + 2;
            __uint128_t d4 = 3 * k + 1;

            binom /= d1; binom /= d2; binom /= d3; binom /= d4;
        }

        __uint128_t result128 = pos - neg;
        return (seq_t)result128;   /* cast to seq_t (fits in 64‑bit) */
    }

    /* generic arbitrary‑precision path */
    seq_t binom = 1;          /* B_k = binomial(4k,k)   */
    seq_t pos   = 0;          /* sum of terms with even sign */
    seq_t neg   = 0;          /* sum of terms with odd  sign */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t tk = binom / ((seq_t)(3 * k + 1));

        if ((n - k) & (seq_t)1)
            neg += tk;
        else
            pos += tk;

        if (k == n) break;

        /* update binom to next value */
        seq_t c1 = (seq_t)(4 * k + 4);
        seq_t c2 = (seq_t)(4 * k + 3);
        seq_t c3 = (seq_t)(4 * k + 2);
        seq_t c4 = (seq_t)(4 * k + 1);

        binom *= c1; binom *= c2; binom *= c3; binom *= c4;

        seq_t d1 = (seq_t)(k + 1);
        seq_t d2 = (seq_t)(3 * k + 3);
        seq_t d3 = (seq_t)(3 * k + 2);
        seq_t d4 = (seq_t)(3 * k + 1);

        binom /= d1; binom /= d2; binom /= d3; binom /= d4;
    }

    return pos - neg;   /* non‑negative result */
}
