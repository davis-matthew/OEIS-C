/*
 * A337553  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (4*k-3) * a(n-k).
 * https://oeis.org/A337553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A337553
 * Recurrence: a(0) = 1;
 *             a(n) = Σ_{k=1..n} binomial(n,k) * (4*k-3) * a(n-k)
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static memoisation cache */
static seq_t *cache = NULL;     /* cache[0..cap-1] */
static size_t  cache_cap = 0;   /* current capacity */

/* Ensure the cache can hold indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failure */

    /* Zero the newly allocated part so uncomputed entries are 0 */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    ensure_cache(n);

    /* a(0) is fixed */
    if (cache[0] == 0) cache[0] = (seq_t)1;

    /* Compute values up to n if they are not yet computed */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum = 0;
        seq_t bin = 1;   /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* bin = C(i,k) */
            bin = bin * (seq_t)(i - k + 1) / (seq_t)k;

            /* coeff = (4*k - 3) */
            seq_t coeff = (seq_t)4 * (seq_t)k - (seq_t)3;

            /* term = bin * coeff * a(i-k) */
            seq_t term = bin * coeff * cache[i - k];

            sum += term;
        }
        cache[i] = sum;
    }

    return cache[n];
}
