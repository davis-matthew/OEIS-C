/*
 * A365752  Expansion of (1/x) * Series_Reversion( x*(1+x)*(1-x)^4 ).
 * https://oeis.org/A365752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(n+k,k) *
 *            binomial(4*n-2*k+2,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static inline uint64_t binom_u64(idx_t nn, idx_t kk)
{
    if (kk > nn) return 0;
    if (kk > nn - kk) kk = nn - kk;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= kk; ++i)
    {
        res = res * (nn - kk + i);
        res = res / i;  /* exact division */
    }
    return (uint64_t)res;
}

seq_t a(idx_t n)
{
    /* If seq_t is 64‑bit or smaller we compute with 128‑bit
       intermediates to avoid overflow.  For larger seq_t
       (arbitrary‑precision) we rely on the provided binom() function. */
    if (sizeof(seq_t) <= 8)
    {
        unsigned __int128 sum = 0;
        idx_t maxk = n / 2;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            uint64_t b1 = binom_u64(n + k, k);
            uint64_t b2 = binom_u64(4 * n - 2 * k + 2, n - 2 * k);
            sum += (unsigned __int128)b1 * b2;
        }
        return (seq_t)(sum / (unsigned __int128)(n + 1));
    }
    else
    {
        seq_t sum = 0;
        idx_t maxk = n / 2;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            seq_t b1 = binom(n + k, k);
            seq_t b2 = binom(4 * n - 2 * k + 2, n - 2 * k);
            sum += b1 * b2;
        }
        return sum / (seq_t)(n + 1);
    }
}
