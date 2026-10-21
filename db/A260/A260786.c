/*
 * A260786  Twice the Euler or up/down numbers A000111.
 * https://oeis.org/A260786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 2 a(n) = (1/4) * Σ_{k=1..n} binomial(n-1, k-1) *
 *            a(k-1) * a(n-k) (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Local cache for memoisation */
static seq_t *cache          = NULL;   /* array of a(i) values          */
static size_t cache_cap      = 0;      /* current allocation capacity (in elements) */
static size_t computed_len   = 0;      /* number of entries already computed (next free index) */

/* Expands the cache so that it can hold indices up to n (inclusive). */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap)
        return;

    /* Allocate more space: at least double the current size or enough for needed. */
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* out of memory – abort the program */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough. */
    grow_cache(n);

    /* Initialise the first two values if they are missing. */
    if (computed_len == 0) {
        cache[0] = (seq_t)2;
        computed_len = 1;
    }
    if (computed_len == 1 && n >= 1) {
        cache[1] = (seq_t)2;
        computed_len = 2;
    }
    /* Compute all missing terms up to n. */
    for (idx_t m = (idx_t)computed_len; m <= n; ++m) {
        if (m == 0 || m == 1) {
            cache[m] = (seq_t)2;
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = 1;                          /* binomial(m-1,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* sum += C(m-1,k-1) * a(k-1) * a(m-k) */
            sum += binom * cache[k - 1] * cache[m - k];

            if (k < m) {
                /* Update binom to C(m-1,k) :  binom * (m - k) / k
                   (division is exact because binomial coefficients are integers) */
                binom = (binom * (seq_t)(m - k)) / (seq_t)k;
            }
        }
        cache[m] = sum / 4;    /* final division by 4 is exact */
    }

    computed_len = (size_t)(n + 1);
    return cache[n];
}
