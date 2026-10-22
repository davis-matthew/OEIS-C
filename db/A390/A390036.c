/*
 * A390036  a(n) = Sum_{k=0..floor(3*n/8)} binomial(k+2,3*n-8*k).
 * https://oeis.org/A390036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊3n/8⌋} binomial(k+2,3n-8k)
 * Range:     u8 n <= 25, u16 n <= 47, u32 n <= 93, u64 n <= 184,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n ≥ 9): a(n) = 3 a(n‑3) – 3 a(n‑6) + a(n‑8) + a(n‑9) */
/* Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=3, a(4)=0, a(5)=0,
 *                 a(6)=6, a(7)=0, a(8)=1. */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache */
static seq_t *cache = NULL;          /* array of computed a(n) values */
static size_t cache_size = 0;        /* allocated size of cache */
static size_t computed_max = (size_t)-1; /* largest n whose value is computed */

/* Ensure cache has room for indices 0..n and initialise first nine values. */
static void ensure_cache(size_t n)
{
    if (cache == NULL) {
        /* Allocate space for at least the first 9 values. */
        size_t init_len = 9;
        seq_t *tmp = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!tmp) abort();

        tmp[0] = (seq_t)1;
        tmp[1] = (seq_t)0;
        tmp[2] = (seq_t)0;
        tmp[3] = (seq_t)3;
        tmp[4] = (seq_t)0;
        tmp[5] = (seq_t)0;
        tmp[6] = (seq_t)6;
        tmp[7] = (seq_t)0;
        tmp[8] = (seq_t)1;

        cache = tmp;
        cache_size = init_len;
        computed_max = 8;
    }

    if (n >= cache_size) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();

        #ifdef __cplusplus
        /* Construct the newly allocated objects for C++ builds. */
        for (size_t i = cache_size; i < new_len; ++i) {
            new (tmp + i) seq_t();
        }
        #endif

        cache = tmp;
        cache_size = new_len;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    size_t sn = (size_t)n;            /* convert to a native integer for loops */

    ensure_cache(sn);

    if (sn < 9) {
        return cache[sn];
    }

    /* Compute all missing values up to sn. */
    for (size_t i = computed_max + 1; i <= sn; ++i) {
        seq_t term1 = (seq_t)3 * cache[i - 3];
        seq_t term2 = (seq_t)3 * cache[i - 6];
        cache[i] = term1 - term2 + cache[i - 8] + cache[i - 9];
    }
    computed_max = sn;

    return cache[sn];
}
