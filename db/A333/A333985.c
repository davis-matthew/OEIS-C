/*
 * A333985  a(0) = 0; a(n) = 6^(n-1) + (1/n) * Sum_{k=1..n-1} binomial(n,k)^2
 *          * 6^(k-1) * (n-k) * a(n-k).
 * https://oeis.org/A333985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n-1) + (1/n) * Sum_{k=1..n-1} binomial(n,k)^2 *
 *            6^(k-1) * (n-k) * a(n-k) a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array holding computed a(n) */
static idx_t cache_size = 0;         /* allocated entries (>= n+1 when needed) */
static idx_t computed = 0;           /* highest n that has been computed */

/* Grow the cache so that it can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = cache_size > 0 ? cache_size * 2 : 1;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }
}

/* Return a(n) using the recurrence described above.  The function
   remembers the values that have already been computed so that
   unordered calls are handled efficiently. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Ensure the cache has been initialized for the base cases. */
    ensure_cache(1);
    if (computed < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* Extend the cache up to the requested n. */
    if (n > computed) {
        ensure_cache(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            seq_t comb = 1;          /* current binomial(n,k) */
            seq_t p   = 1;           /* 6^(k-1) */
            for (idx_t k = 1; k < i; ++k) {
                /* Compute binomial(i, k) incrementally. */
                comb = (comb * (seq_t)(i - k + 1)) / (seq_t)k;
                seq_t bin_sq = comb * comb;
                seq_t term   = bin_sq * p;
                term = term * (seq_t)(i - k);
                term = term * cache[i - k];
                sum += term;
                p *= (seq_t)6;       /* next power of 6 */
            }
            seq_t base = p;          /* 6^(i-1) */
            cache[i] = base + sum / (seq_t)i;
        }
        computed = n;
    }

    return cache[n];
}
