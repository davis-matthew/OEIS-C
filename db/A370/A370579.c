/*
 * A370579  a(n) = n! * Sum_{d|n} 1/(d-1)!.
 * https://oeis.org/A370579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{d|n} 1/(d-1)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate factorial array: fact[i] = i!  (i=0..n) */
    size_t sz = (size_t)(n + 1);
    seq_t *fact = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fact) return 0;          /* fallback, should not happen in tests */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t fn  = fact[n];
    seq_t res = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d = i;
            seq_t term = fn / fact[d - 1];
            res += term;

            idx_t k = n / d;
            if (k != d)
            {
                term = fn / fact[k - 1];
                res += term;
            }
        }
    }

    free(fact);
    return res;
}
