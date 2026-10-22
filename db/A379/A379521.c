/*
 * A379521  Expansion of (1/x) * Series_Reversion( x / ( (1+x)^3 * (1+2*x)^2 )
 *          ).
 * https://oeis.org/A379521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 2^k * binomial(2*(n+1),k) *
 *            binomial(3*(n+1),n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"


seq_t a(idx_t n)
{
    idx_t m       = n + 1;          /* m = n+1 */
    idx_t two_m   = 2 * m;          /* 2*(n+1)       */
    idx_t three_m = 3 * m;          /* 3*(n+1)       */

    /* binomial(2*m,k)   starts at k=0 */
    seq_t binom_2m_k = 1;

    /* binomial(3*m,n-k) starts at k=0 (i.e., n-k = n) */
    seq_t binom_3m_n_k = binomial(three_m, n);

    /* Use 128‑bit arithmetic for the intermediate sum to avoid overflows
       when seq_t is 64‑bit.  The final value fits in seq_t whenever the
       final sequence term does. */
    unsigned __int128 sum  = 0;
    unsigned __int128 pow2 = 1;   /* 2^k */

    for (idx_t k = 0; k <= n; ++k)
    {
        unsigned __int128 term = pow2;
        term *= (unsigned __int128)binom_2m_k;
        term *= (unsigned __int128)binom_3m_n_k;
        sum += term;

        if (k == n) break;          /* no further update needed */

        /* update binom_2m_k to C(2*m, k+1) */
        binom_2m_k = binom_2m_k * ((seq_t)(two_m - k)) / ((seq_t)(k + 1));

        /* update binom_3m_n_k to C(3*m, n-(k+1)) */
        idx_t h = n - k;            /* current argument = n-k */
        binom_3m_n_k = binom_3m_n_k * ((seq_t)h) /
                       ((seq_t)(three_m - h + 1));

        pow2 <<= 1;                 /* next power of two */
    }

    /* Final division by (n+1) with 128‑bit precision, then cast back. */
    seq_t result = (seq_t)(sum / (unsigned __int128)m);
    return result;
}
