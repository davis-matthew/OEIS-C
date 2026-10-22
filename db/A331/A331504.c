/*
 * A331504  Number of labeled graphs with n nodes and floor(n*(n-1)/4) edges.
 * https://oeis.org/A331504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(n,2), floor(n*(n-1)/4) )
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"


/* Safe multiplicative binomial using gcd reduction */
static seq_t binomial_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* ≤ n */
        seq_t den = i;           /* ≤ k ≤ n/2 */

        /* Reduce result and denominator */
        seq_t d = gcd(result, den);
        result /= d;
        den /= d;

        /* Reduce numerator and remaining denominator */
        d = gcd(num, den);
        num /= d;
        den /= d;

        /* At this point den should be 1 */
        result *= num;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Total possible edges: binomial(n,2) */
    idx_t total_edges = n * (n - 1) / 2;

    /* Desired number of edges: floor(n*(n-1)/4) */
    idx_t k = n * (n - 1) / 4;

    return binomial_safe(total_edges, k);
}
