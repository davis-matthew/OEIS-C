/*
 * A320232  Expansion of Product_{k=1..6} theta_3(q^k), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A320232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of integer solutions (a1,…,a6) to * a1^2 +
 *            2*a2^2 + 3*a3^2 + 4*a4^2 + 5*a5^2 + 6*a6^2 = n. * This is
 *            implemented by a simple dynamic‑programming scan for each k
 * Range:     u8 n <= 20, u16 n <= 299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is 1: the zero vector */
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;               /* allocation failed */

    dp[0] = (seq_t)1;                  /* base case */

    for (idx_t k = 1; k <= 6; ++k)
    {
        /* allocate new array for this k */
        seq_t *newdp = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!newdp)
        {
            free(dp);
            return (seq_t)0;           /* allocation failed */
        }

        for (size_t s = 0; s <= N; ++s)
        {
            seq_t ways = dp[s];
            if (!ways)
                continue;

            /* iterate over integer a such that k*a*a <= N-s */
            for (idx_t a = 0;; ++a)
            {
                idx_t v = k * a * a;
                if (v > (idx_t)(N - s))
                    break;

                /* multiplicity: 1 for a==0, 2 for a!=0 */
                seq_t mult = (a == 0) ? (seq_t)1 : (seq_t)2;
                newdp[s + v] += ways * mult;
            }
        }

        free(dp);
        dp = newdp;
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
