/*
 * A341312  a(n) = a(n-1) + a(n-3) unless a(n-1) and a(n-3) are both even in
 *          which case a(n) = (a(n-1) + a(n-3))/2, with a(0) = a(1) = a(2) =
 *          1.
 * https://oeis.org/A341312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = a(2) = 1 For n ≥ 3: a(n) = a(n−1) + a(n−3) if
 *            a(n−1) and a(n−3) are both even then a(n) = (a(n−1) + a(n−3)) /
 *            2 (OEIS Offset: 0)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 76, u64 n <= 154,
 *            u128 n <= 301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache holds the sequence values up to the largest n
       computed so far.  The cache is grown on demand.  All indices
       used are non‑negative, so size_t is safe for array indices. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0; /* allocated capacity */
    static size_t cache_len = 0; /* number of entries already computed */

    /* Handle trivial cases directly; no cache needed yet. */
    if (n < 3) return (seq_t)1;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 4;                  /* room for indices 0..3 initially */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_len = 3;                  /* 0,1,2 are known */
    }

    /* Ensure we have enough room for indices up to n. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t sum = a1 + a3;
        if ((a1 & (seq_t)1) == 0 && (a3 & (seq_t)1) == 0) {
            sum /= 2;
        }
        cache[i] = sum;
    }

    /* Update length of computed entries. */
    cache_len = (size_t)n + 1;
    return cache[n];
}
