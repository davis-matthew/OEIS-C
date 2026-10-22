/*
 * A378484  Expansion of (Sum_{k>=0} binomial(4*k,k) * x^k)^4.
 * https://oeis.org/A378484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(k+2,2) * binomial(4*n+3, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

/* Multiplicative binomial coefficient.
   It works for all idx_t supplied limits, and returns the exact result
   in seq_t.  Division is always exact, so truncating / is safe. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;               /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return 1;                  /* a(0) = 1 */

    seq_t sum     = 0;
    seq_t power3  = 1;              /* 3^k, start with k = 0 */
    seq_t binom2  = binomial(4 * n + 3, n);  /* C(4n+3, n-k) for k = 0 */
    idx_t j       = n;              /* current lower parameter for binom2 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Three objects that fit together: 3^k, C(k+2,2), C(4n+3, n-k) */
        seq_t c2 = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;  /* C(k+2,2) */

        /* term = power3 * c2 * binom2 */
        seq_t term = power3;
        term *= c2;
        term *= binom2;
        sum += term;

        if (k == n)               /* last iteration – no update needed */
            break;

        /* Update for next k */
        power3 *= 3;               /* 3^(k+1) */

        /* Update binom2 from C(N, j) to C(N, j-1) with N = 4n+3 */
        seq_t numer = binom2 * (seq_t)j;
        seq_t denom = (seq_t)(4 * n + 3 - j + 1);
        binom2 = numer / denom;
        --j;
    }

    return sum;
}
