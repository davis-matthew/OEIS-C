/*
 * A386897  a(n) = 4^n * binomial(5*n/2,n).
 * https://oeis.org/A386897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(5*n+1,k) * binomial(4*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            which equals 4^n * binomial(5*n/2,n) for integer n. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return 1;            /* a(0) = 1 */

    idx_t top1 = 5 * n + 1;   /* for C(5*n+1, k) */
    idx_t top2 = 4 * n;       /* for C(4*n-k, n-k) */

    /* Initial values for k = 0 */
    seq_t binom1 = 1;                                 /* C(5*n+1, 0) */
    seq_t binom2 = binomial(top2, n);                 /* C(4*n, n) */
    sum = binom1 * binom2;

    /* Iterate k = 1 .. n, updating both binomials multiplicatively */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* C(5*n+1, k) = C(5*n+1, k-1) * (5*n+1-(k-1)) / k */
        binom1 = binom1 * (top1 - (k - 1)) / k;

        /* C(4*n-k, n-k) = C(4*n-(k-1), n-(k-1)) * (n-(k-1)) / (4*n-(k-1)) */
        binom2 = binom2 * (n - (k - 1)) / (top2 - (k - 1));

        sum += binom1 * binom2;
    }
    return sum;
}
