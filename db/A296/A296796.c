/*
 * A296796  Numbers k such that k is the altitude of a Heronian triangle with
 *          sides m - 11, m, m + 11.
 * https://oeis.org/A296796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) - a(n-6) for n > 5 Initial values: a(0)=12,
 *            a(1)=15, a(2)=33, a(3)=63, a(4)=72, a(5)=132
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 44, u64 n <= 94, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_n = 0;
static size_t cap = 0;

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }
}

/* Compute a(n) with memoization. */
seq_t a(idx_t n)
{
    /* Initial terms. */
    static const seq_t init_vals[6] = {
        (seq_t)12, (seq_t)15, (seq_t)33,
        (seq_t)63, (seq_t)72, (seq_t)132
    };

    if (cache == NULL) {
        /* Allocate initial cache and store first six values. */
        ensure_capacity(5);
        for (idx_t i = 0; i <= 5; ++i) cache[i] = init_vals[i];
        cached_n = 5;
    }

    if (n <= cached_n) return cache[n];

    /* Extend cache up to n. */
    ensure_capacity(n);
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i - 3] - cache[i - 6];
    }
    cached_n = n;
    return cache[n];
}
