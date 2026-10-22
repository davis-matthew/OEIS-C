/*
 * A334626  G.f.: Sum_{k>=0} x^(k^3) / Product_{j=1..k^3} (1 - x^j).
 * https://oeis.org/A334626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1, k^3<=n} (p(n,k^3)-p(n,k^3-1)), a(0)=1 where
 *            p(n,m) = number of partitions of n with all parts <= m
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* out‑of‑memory guard */

    /* Initialise dp: dp[i] = partitions of i with parts <= current j */
    for (idx_t i = (idx_t)1; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* one way to partition 0 */

    seq_t result = (seq_t)0;

    /* Iterate over all part sizes up to n, keeping track of cubes */
    idx_t cube     = (idx_t)1;      /* current cube root */
    idx_t nextCube = cube * cube * cube;  /* current cube value */

    for (idx_t j = (idx_t)1; j <= n; ++j)
    {
        /* Save count before adding part size j */
        seq_t before_n = dp[n];

        /* Update dp: add part of size j */
        for (idx_t i = j; i <= n; ++i)
            dp[i] = dp[i] + dp[i - j];

        /* If j is a cube, add partitions with largest part exactly j */
        if (j == nextCube)
        {
            seq_t after_n = dp[n];
            result = result + (after_n - before_n);

            /* Advance to the next cube */
            cube++;
            nextCube = cube * cube * cube;
        }
    }

    free(dp);
    return result;
}
