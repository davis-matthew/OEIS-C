/*
 * A375224  Expansion of e.g.f. exp( x^2/(1-x)^3 ) / (1-x)^2.
 * https://oeis.org/A375224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} binomial(n+1+k, n-2*k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* -------------------------------------------------------------- */
/* auxiliary binomial coefficient:   binomial(N, K)              */
/* -------------------------------------------------------------- */
static seq_t binomial(const idx_t N, const idx_t K)
{
    if (K > N) return 0;
    idx_t k = K;
    if (k > N - k) k = N - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by (N - k + i) then divide by i; the division is exact */
        res = res * (seq_t)(N - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* -------------------------------------------------------------- */
/* sequence a(n) as defined in the problem statement             */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t const maxk = n >> 1;

    /* n!  (computed once) */
    seq_t fac_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fac_n = fac_n * (seq_t)i;

    /* k!  , updated incrementally in the loop */
    seq_t fac_k = 1;          /* 0! = 1 */

    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin = binomial(n + 1 + k, n - 2 * k);
        seq_t term = (bin * fac_n) / fac_k;
        sum += term;

        /* update k! for next iteration */
        fac_k = fac_k * (seq_t)(k + 1);
    }

    return sum;
}
