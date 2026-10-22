/*
 * A348862  G.f. A(x) satisfies: A(x) = 1 / ((1 + x) * (1 - x * A(4*x))).
 * https://oeis.org/A348862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} 4^k * a(k) * a(n-k-1). Initial
 *            values: a(0)=1, a(1)=0. This implementation uses memoisation to
 *            compute the sequence on demand
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap    = 0;
static idx_t  max_computed = 0;  /* index of the largest value that has
                                   been computed (0 means a(0) not yet
                                   stored). */

static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cap)
        return;
    size_t new_cap = (n + 1) * 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* out of memory */
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Guarantee that a(0) is set when we start computing. */
    if (max_computed == 0)
        cache[0] = (seq_t)1;

    if (max_computed < n) {
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            seq_t sum  = (seq_t)0;
            seq_t pow4 = (seq_t)1;          /* 4^k */

            for (idx_t k = 0; k < m; ++k) {
                sum += pow4 * cache[k] * cache[m - 1 - k];
                pow4 *= (seq_t)4;
            }

            /* add (-1)^m */
            if (m & (idx_t)1)
                sum -= (seq_t)1;
            else
                sum += (seq_t)1;

            cache[m] = sum;
        }
        max_computed = n;
    }
    return cache[n];
}
