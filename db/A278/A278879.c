/*
 * A278879  a(n) = Sum_{k=0..n-1} (k+1)*binomial(n,k) * a(k) for n > 0 with
 *          a(0) = 1.
 * https://oeis.org/A278879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (k+1)*binomial(n,k)*a(k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already computed values */
static seq_t *a_cache = NULL;      /* array of a(k) */
static size_t cache_cap = 0;       /* allocated capacity (number of elements) */
static idx_t  computed_max = 0;    /* largest n whose value is known */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (a_cache == NULL) {
        /* first allocation */
        cache_cap = (n > 0) ? 1 << 1 : 1;
        while (cache_cap <= (size_t)n) cache_cap <<= 1;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache) abort(); /* out of memory */
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!a_cache) abort();
        cache_cap = new_cap;
    }
}

/* Return a(n) for arbitrary n, using memoization */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) {
        ensure_cache(0);
        if (a_cache[0] == 0) a_cache[0] = 1;
        computed_max = 0;
        return a_cache[0];
    }

    /* make sure we have enough space */
    ensure_cache(n);

    /* initialise a(0) if it hasn't been set yet */
    if (a_cache[0] == 0) {
        a_cache[0] = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                    /* C(i,0) = 1 */
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = bin * a_cache[k];
            term = term * (seq_t)(k + 1);
            sum += term;
            /* Update bin to C(i,k+1) */
            bin = bin * (seq_t)(i - k);
            bin = bin / (seq_t)(k + 1);   /* exact division */
        }
        a_cache[i] = sum;
    }
    computed_max = n;
    return a_cache[n];
}
