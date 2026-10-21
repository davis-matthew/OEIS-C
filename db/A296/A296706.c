/*
 * A296706  Numbers whose base-8 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th natural number with equal number of rises and falls
 *            * in its base‑8 expansion. The function searches forward from
 *            the last * candidate, caching results so that repeated calls (in
 *            any order) are * efficient
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool ok(seq_t x)
{
    seq_t t = x;
    int prev = (int)(t % 8);
    t /= 8;
    int rises = 0, falls = 0;
    while (t != (seq_t)0) {
        int cur = (int)(t % 8);
        if (cur < prev)
            ++falls;
        else if (cur > prev)
            ++rises;
        prev = cur;
        t /= 8;
    }
    return rises == falls;
}

seq_t a(idx_t n)
{
    /* Small array to store computed sequence elements. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;

    /* Return already computed value. */
    if (n == 0)
        return 0;                      /* undefined for n < 1, but safe */
    if ((size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];

    /* Grow cache as needed. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    for (; cache_size < (size_t)n; ++next_candidate) {
        if (ok(next_candidate)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
    }
    return cache[(size_t)(n - 1)];
}
