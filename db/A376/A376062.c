/*
 * A376062  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), a(3), ... such that for any n > 0, S(n) = Sum_{k = 1..n}
 *          b(k)/a(k) < 1, where {b(k)} is the sequence {7/6, 5/4, 5/4, 5/4,
 *          ...}.
 * https://oeis.org/A376062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 4, a(n+1) = a(n)*a(n) - a(n) + 1 for n >= 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A376062.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of cache array */

    if (n == 0)   /* out of range; seq_t may be unsigned */
        return (seq_t)0;

    /* allocate or enlarge cache to hold at least n terms */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* on allocation failure tmp becomes NULL; keep old cache */
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* compute terms that have not yet been stored */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;          /* a(1) */
        cache_len = 1;
        if (n == 1) return cache[0];
    }
    if (cache_len == 1 && n >= 2) {
        cache[1] = (seq_t)4;          /* a(2) */
        cache_len = 2;
        if (n == 2) return cache[1];
    }

    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        cache[cache_len] = prev * prev - prev + 1;
        ++cache_len;
    }

    return cache[n - 1];
}
