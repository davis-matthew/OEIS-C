/*
 * A390901  Expansion of e.g.f. Series_Reversion(3*x + log(1-2*x)).
 * https://oeis.org/A390901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = -2*a(n-1) + 3*Sum_{k=1..n-1}
 *            binomial(n,k)*a(k)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle zero or negative indices safely (undefined offset). */
    if (n == 0) return (seq_t)0;

    /* Static cache: a[0] is unused, a[1]..a[n] are stored. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated slots (including index 0) */
    static idx_t cache_max = 0;           /* highest index computed so far */

    /* Allocate or grow cache to hold indices up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;   /* grow factor 2 to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        if (m == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;  /* binomial(m,0) */

        for (idx_t k = 1; k < m; ++k) {
            /* Update bin = bin * (m-k+1) / k  (exact division). */
            bin = bin * (seq_t)(m - k + 1);
            bin = bin / (seq_t)k;

            /* Add bin * a(k) * a(m-k) to sum. */
            sum += bin * cache[k] * cache[m - k];
        }

        /* a(m) = 3*sum - 2*a(m-1)   (eq. to -2*a(m-1)+3*sum) */
        seq_t term1 = 3 * sum;
        seq_t term2 = 2 * cache[m - 1];
        cache[m] = term1 - term2;  /* term1 >= term2 for this sequence */
    }

    cache_max = n;
    return cache[n];
}
