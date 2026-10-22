/*
 * A391733  Numbers in whose binary expansion there is exactly one run of
 *          1-bits with an odd length, while all other runs of 1-bits are of
 *          an even length (or absent).
 * https://oeis.org/A391733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 7852, u32 n <= 9840, u64 n <= 9840,
 *            u128 n <= 9840, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Search sequential integers, keep those whose binary
 * representation contains exactly one run of 1‑bits with odd length
 * (all other runs of 1‑bits are even).  Results are memoized so
 * later calls to a() are fast. */
#include <stdlib.h>  /* for malloc, realloc */

static int is_good(seq_t x)
{
    int odd_runs = 0;
    while (x) {
        /* skip zeros between runs */
        while (!(x & (seq_t)1)) {
            x >>= 1;
        }
        /* count consecutive 1's */
        int len = 0;
        while (x & (seq_t)1) {
            len++;
            x >>= 1;
        }
        if (len & 1) {
            odd_runs++;
            if (odd_runs > 1) {
                return 0;
            }
        }
    }
    return odd_runs == 1;
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;

    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* initialize next_candidate on first call */
    if (cache_len == 0) {
        next_candidate = 1;
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        while (!is_good(next_candidate)) {
            next_candidate++;
        }
        /* store the good number */
        if (cache_len == cache_cap) {
            cache_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = next_candidate;
        next_candidate++;   /* next number to test */
    }

    return cache[n - 1];
}
