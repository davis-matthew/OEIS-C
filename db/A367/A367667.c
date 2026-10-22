/*
 * A367667  G.f. A(x) satisfies A(x) = 1 / (1 - x - x^3 * A(x^3)).
 * https://oeis.org/A367667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 1; a(2) = 1; a(n) = a(n-1) + Σ_{k=0..⌊(n-3)/3⌋}
 *            a(k) * a(n-3-3k) (n ≥ 3)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values    */
static idx_t cache_len = 0;          /* number of elements in cache */
static idx_t computed_up_to = (idx_t)-1; /* highest index with value   */

/* grow the cache so it can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    size_t new_len = n + 1;
    if (new_len < 16)          /* start with a small block              */
        new_len = 16;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)                 /* fatal error – terminate program */
        exit(1);

    cache = tmp;
    cache_len = new_len;
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* initialise base values on first use */
    if (computed_up_to == (idx_t)-1) {
        cache[0] = 1;
        if (cache_len > 1) cache[1] = 1;
        if (cache_len > 2) cache[2] = 1;
        computed_up_to = 2;
    }

    /* if already computed, just return */
    if (n <= computed_up_to)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];   /* start with a(i-1)                */

        if (i >= 3) {
            idx_t limit = (i - 3) / 3;
            seq_t sum = 0;
            for (idx_t k = 0; k <= limit; ++k) {
                seq_t prod = cache[k] * cache[i - 3 - 3 * k];
                sum += prod;
            }
            val += sum;
        }

        cache[i] = val;
    }

    computed_up_to = n;
    return cache[n];
}
