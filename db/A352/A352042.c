/*
 * A352042  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-2*k-1,k) *
 *          a(k).
 * https://oeis.org/A352042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-2*k-1,k) *
 *            a(k)
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 52, u64 n <= 99, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the values of the sequence.  For each new
 * maximum requested index it grows a static array and computes all
 * missing terms using the recurrence.  The binomial coefficient is
 * computed multiplicatively to avoid external utilities.  All arithmetic
 * uses the seq_t type supplied by the build system. */

#include <stdlib.h>   /* malloc, realloc */

/* Helper to compute binomial(n, k) as seq_t (k <= n). */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);  /* multiply by numerator */
        res = res / i;            /* divide by denominator (exact) */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Static cache of the sequence values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed  = 0;   /* highest index computed so far */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* fallback on allocation failure */
        cache_cap = 1;
        cache[0] = 1;      /* a(0) = 1 */
        computed  = 0;
    }

    /* Grow cache if needed to fit index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
        /* No need to initialise new entries; they will be written when computed */
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 0) continue;   /* already initialised */

        idx_t limit = (i - 1) / 3;          /* floor((i-1)/3) */
        seq_t sum = 0;

        for (idx_t k = 0; k <= limit; ++k) {
            seq_t bin  = binom_idx(i - 2 * k - 1, k);
            sum += bin * cache[(size_t)k];
        }
        cache[(size_t)i] = sum;
    }
    computed = n;
    return cache[(size_t)n];
}
