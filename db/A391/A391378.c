/*
 * A391378  Expansion of g^2/(1 + x*g), where g = 1+x*g^4 is the g.f. of
 *          A002293.
 * https://oeis.org/A391378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (k+2) * binomial(4*n-3*k+2,
 *            n-k)/(4*n-3*k+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* 128‑bit binomial for 64‑bit builds only.
 * Works for all arguments that fit into 128 bits (always true for n <= 10000
 * when the final term fits into 64 bits).
 */
static inline unsigned __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    unsigned __int128 result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (unsigned __int128)(n + 1 - i);
        result /= (unsigned __int128)i;   /* exact division */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Path for 64‑bit integer builds */
#if SIZEOF_SEQ_T <= 8
    signed __int128 sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t denom = 4 * n - 3 * k + 2;
        idx_t r = n - k;

        unsigned __int128 bin = binom128(denom, r);
        unsigned __int128 term = (unsigned __int128)(k + 2) * bin;
        term /= (unsigned __int128)denom;

        if (k & 1)
            sum -= (signed __int128)term;
        else
            sum += (signed __int128)term;
    }
    return (seq_t)sum;

#else   /* arbitrary‑precision builds */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t denom = 4 * n - 3 * k + 2;
        idx_t r = n - k;

        seq_t bin = binomial(denom, r);
        seq_t term = ((seq_t)(k + 2) * bin) / denom;

        if (k & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
#endif
}
