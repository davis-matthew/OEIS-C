/*
 * A338301  O.g.f. satisfies: A(x) = 1/(1 - x - x*( d/dx x^2*A(x)' )).
 * https://oeis.org/A338301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338301
 *
 * This sequence satisfies the functional equation
 *      A(x) = 1 / ( 1 - x - Σ_{m≥1} m(m-1) a_{m-1} x^m ),
 * where A(x)=∑_{n≥0} a_n x^n.
 *
 * From this one obtains the recurrences
 *      a(0) = 1
 *      a(n) = a(n-1) + Σ_{i=2}^n i(i-1) a(i-1) a(n-i)   (n ≥ 1)
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/* Memoisation storage */
static seq_t *cache = NULL;      /* array storing a(0), a(1), ... */
static idx_t cache_len = 0;      /* number of terms currently computed */
static idx_t cache_cap = 0;      /* allocated capacity of cache */

/* Ensure the cache can hold up to index n */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap) return;

    idx_t new_cap = (n + 1) * 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;      /* allocation failure – should not happen in tests */

    /* Copy existing values */
    for (idx_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* Free old cache */
    free(cache);

    cache       = new_cache;
    cache_cap   = new_cap;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) {                  /* cache may be empty here */
        ensure_capacity(0);
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    ensure_capacity(n);

    /* Initialise a(0) if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute terms up to n */
    while (cache_len <= n) {
        idx_t k = cache_len;          /* index of the term to compute */
        seq_t sum = cache[k-1];       /* start with a(k-1) */

        for (idx_t i = 2; i <= k; ++i) {
            seq_t factor = ((seq_t)i) * ((seq_t)(i-1));
            seq_t term   = factor * cache[i-1] * cache[k-i];
            sum += term;
        }

        cache[k] = sum;
        ++cache_len;
    }

    return cache[n];
}
