/*
 * A304422  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n > 10): a(n) = 4*a(n-1) +
 *            15*a(n-2) + 7*a(n-3) - 36*a(n-4) - 72*a(n-5) - 51*a(n-6) -
 *            29*a(n-7) - 19*a(n-8) - 4*a(n-9) Initial values (a(1) … a(10)):
 *            1, 25, 124, 836, 5227, 33263, 210954, 1338995, 8497693, 53931843
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static size_t cache_alloc = 0;       /* size of allocated array (index 0 unused) */
static idx_t cache_max  = 0;         /* highest index for which a value is stored */

/* Ensure the cache can hold indices up to n (inclusive). */
static void expand_cache(idx_t n)
{
    size_t new_size = cache_alloc ? cache_alloc * 2 : 16;
    while (new_size <= (size_t)n) {
        new_size *= 2;
    }
    cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    cache_alloc = new_size;
}

/* Return the n-th term of sequence A304422 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but harmless */

    /* If we already have computed terms up to n, return the stored value. */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Ensure sufficient storage. */
    if ((size_t)n >= cache_alloc) {
        expand_cache(n);
    }

    /* Initial terms for n = 1 … 10. */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)25,
        (seq_t)124,
        (seq_t)836,
        (seq_t)5227,
        (seq_t)33263,
        (seq_t)210954,
        (seq_t)1338995,
        (seq_t)8497693,
        (seq_t)53931843
    };

    /* Compute all terms up to n, filling the cache. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i - 1];
        } else {
            cache[i] =
                (seq_t)4  * cache[i - 1] +
                (seq_t)15 * cache[i - 2] +
                (seq_t)7  * cache[i - 3] -
                (seq_t)36 * cache[i - 4] -
                (seq_t)72 * cache[i - 5] -
                (seq_t)51 * cache[i - 6] -
                (seq_t)29 * cache[i - 7] -
                (seq_t)19 * cache[i - 8] -
                (seq_t)4  * cache[i - 9];
        }
    }

    cache_max = n;
    return cache[n];
}
