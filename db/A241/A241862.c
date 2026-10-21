/*
 * A241862  Number of compositions of n such that the smallest part has
 *          multiplicity two.
 * https://oeis.org/A241862
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊n/2⌋} ( n==2m ? 1 : Σ_{k=1}^{⌊(n-2m)/(m+1)⌋}
 *            C(k+2,2)·C( n-2m-k·(m+1)+k-1 , k-1 ) )
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 174,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute C(n,k) using a multiplicative algorithm with gcd‑reduction.
   Works for all supported seq_t sizes. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);   /* numerator term */
        seq_t den = (seq_t)i;             /* denominator term */

        seq_t g = gcd(result, den);
        result = result / g;
        den = den / g;

        g = gcd(num, den);
        num = num / g;
        den = den / g;

        result = result * num;
        if (den > 1)
            result = result / den;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* sequence is defined for n ≥ 2 */

    seq_t ans = 0;

    for (idx_t m = 1; 2 * m <= n; ++m)
    {
        idx_t t = n - 2 * m;             /* remaining sum after two m's */

        if (t == 0)                      /* composition [m,m] only */
        {
            ans += 1;
            continue;
        }

        idx_t min_part = m + 1;          /* smallest allowed part > m */
        idx_t k_max = t / min_part;      /* maximum number of larger parts */

        for (idx_t k = 1; k <= k_max; ++k)
        {
            idx_t u = t - k * min_part;  /* non‑negative leftover */

            /* C(k+2,2) = (k+2)*(k+1)/2 */
            seq_t binom1 = (seq_t)(k + 2) * (seq_t)(k + 1) / 2;

            /* C(u+k-1 , k-1) */
            seq_t binom2 = binom(u + k - 1, k - 1);

            ans += binom1 * binom2;
        }
    }

    return ans;
}
