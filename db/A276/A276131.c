/*
 * A276131  a(n) =
 *          (a(n-1)^2+a(n-2)^2+a(n-3)^2+a(n-4)^2+a(n-1)*a(n-2)+a(n-1)*a(n-3)+a(n-1)*a(n-4)+a(n-2)*a(n-3)+a(n-2)*a(n-4)+a(n-3)*a(n-4))/a(n-5)
 *          with five initial ones.
 * https://oeis.org/A276131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            15*a(n-1)*a(n-2)*a(n-3)*a(n-4)-a(n-1)-a(n-2)-a(n-3)-a(n-4)-a(n-5)
 *            Initial values: a(0)=a(1)=a(2)=a(3)=a(4)=1 Recurrence from the
 *            OEIS description: a(n) = (a(n-1)^2 + … + a(n-3)*a(n-4)) / a(n-5)
 *            for n ≥ 5 This implementation uses the equivalent closed form to
 *            reduce the number of arithmetic...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;      /* array of computed terms   */
static idx_t  cache_cap = 0;     /* allocated capacity        */
static idx_t  computed = 0;      /* number of terms that are
                                   currently stored (0…computed-1) */

/* Ensure that cache has capacity for indices 0 … n (inclusive). */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (needed <= cache_cap)
        return;
    /* grow capacity (doubling, at least 16) */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    /* realloc and cast explicitly for C++ compliance */
    seq_t *tmp = (seq_t *)realloc(cache,
                                 (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
    /* initialise the first five terms only once */
    if (computed < 5) {
        for (idx_t i = computed; i <= 4; ++i)
            cache[i] = (seq_t)1;
        computed = 5;
    }
}

/* public function computing the sequence */
seq_t a(idx_t n)
{
    if (n < 5)            /* offset 0 – first five terms are 1 */
        return (seq_t)1;

    ensure_cache(n);

    /* compute any missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* use the closed form: 15*a(i-1)*a(i-2)*a(i-3)*a(i-4)
           - a(i-1) - a(i-2) - a(i-3) - a(i-4) - a(i-5) */
        seq_t prod = cache[i-1];
        prod *= cache[i-2];
        prod *= cache[i-3];
        prod *= cache[i-4];
        prod *= (seq_t)15;
        prod -= cache[i-1];
        prod -= cache[i-2];
        prod -= cache[i-3];
        prod -= cache[i-4];
        prod -= cache[i-5];
        cache[i] = prod;
    }
    computed = n + 1;   /* now all indices below n are known */
    return cache[n];
}
