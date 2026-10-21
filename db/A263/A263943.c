/*
 * A263943  Positive integers n such that (n+21)^3 - n^3 is a square.
 * https://oeis.org/A263943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+254*a(n-2)-254*a(n-3)-a(n-4)+a(n-5) (n>5) with
 *            a(1)=7, a(2)=119, a(3)=4564, a(4)=32900, a(5)=1161895
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* cache of computed sequence values; index 0 unused (dummy). */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* First five non‑zero terms (1‑based). */
static const seq_t init_vals[5] = {
    (seq_t)7,
    (seq_t)119,
    (seq_t)4564,
    (seq_t)32900,
    (seq_t)1161895
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds */

    /* allocate or grow the cache to hold indices 0..n */
    size_t needed = (size_t)n + 1;
    size_t old_len = cache_len;
    if (cache_len < needed) {
        seq_t *p;
        if (cache == NULL) {
            p = seq_alloc(needed);
        } else {
            p = seq_realloc(cache, cache_len, needed);
        }
        if (p == NULL) return (seq_t)0;   /* allocation failed */
        cache = p;
        cache_len = needed;
    }

    /* keep dummy 0‑th entry for bounds safety */
    cache[0] = (seq_t)0;

    /* start computing missing terms */
    idx_t start = (idx_t)old_len;
    if (start < 1) start = 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i <= 5) {
            cache[i] = init_vals[i - 1];
        } else {
            cache[i] = cache[i - 1]
                     + ((seq_t)254 * cache[i - 2])
                     - ((seq_t)254 * cache[i - 3])
                     - cache[i - 4]
                     + cache[i - 5];
        }
    }

    return cache[n];
}
