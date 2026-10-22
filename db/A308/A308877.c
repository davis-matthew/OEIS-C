/*
 * A308877  Expansion of e.g.f. (1 + log(1 - x))/(1 + 2*log(1 - x)).
 * https://oeis.org/A308877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} |Stirling1(n,k)| * 2^(k-1) * k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* cached a(0..cache_n) */
    static size_t cache_cap = 0;
    static idx_t  cache_n   = 0;

    static seq_t *prev = NULL; /* previous row T(n-1,·) */
    static seq_t *curr = NULL; /* current row T(n,·)   */
    static size_t row_cap = 0; /* allocated length of prev/curr */
    static idx_t  row_n   = 0; /* n represented by prev */

    /* Handle the trivial case n==0 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate or grow the cache to hold values up to n */
    if (!cache) {
        cache = seq_alloc(n + 1);
        cache_cap = n + 1;
    } else if (n + 1 > cache_cap) {
        cache = seq_realloc(cache, cache_cap, n + 1);
        cache_cap = n + 1;
    }

    /* Initialize the first two entries of the cache if needed */
    if (cache_n < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_n = 1;
    }

    /* If we have already computed a(n), return it */
    if (n <= cache_n)
        return cache[n];

    /* Allocate space for the first rows of T if it hasn't been done yet */
    if (!prev || !curr) {
        prev   = seq_alloc(2);
        curr   = seq_alloc(2);
        row_cap = 2;
        /* T(1,1) = 1, all other entries 0 */
        prev[0] = (seq_t)0;
        prev[1] = (seq_t)1;
        row_n   = 1; /* prev represents n = 1 */
    }

    /* Build rows of T up to the desired n */
    while (row_n < n) {
        idx_t m = row_n + 1; /* compute row for n = m */

        /* Ensure curr has room for indices 0..m */
        if (row_cap < (size_t)(m + 1)) {
            curr = seq_realloc(curr, row_cap, m + 1);
            row_cap = m + 1;
        }

        curr[0] = (seq_t)0;
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= m; ++k) {
            /* term1 = 2*k * prev[k-1] */
            seq_t term1 = seq_mul(prev[k - 1], (seq_t)(2 * k));

            /* term2 = (m-1) * prev[k], if k <= m-1 */
            seq_t term2 = (seq_t)0;
            if (k <= m - 1)
                term2 = seq_mul(prev[k], (seq_t)(m - 1));

            curr[k] = term1 + term2;
            sum += curr[k];
        }

        /* Cache the result for a(m) */
        cache[m] = sum;
        if (m > cache_n)
            cache_n = m;

        /* Swap prev and curr for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;

        ++row_n;
    }

    return cache[n];
}
