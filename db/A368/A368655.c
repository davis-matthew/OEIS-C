/*
 * A368655  Binomial transform of Gould's sequence (A001316).
 * https://oeis.org/A368655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^{popcount(k)} (OEIS
 *            A368655)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* popcount for idx_t (uint64_t) */
static inline unsigned popcount_u64(idx_t x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    seq_t sum  = (seq_t)0;
    seq_t binom = (seq_t)1;             /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow2 = ((seq_t)1) << popcount_u64(k);   /* 2^{popcount(k)} */
        sum += binom * pow2;

        if (k < n)
        {
            /* C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }

    return sum;
}
