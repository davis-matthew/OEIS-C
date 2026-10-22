/*
 * A330971  Numbers k such that for any decimal digit d in k, the remainder
 *          when k is divided by d+1 is 0.
 * https://oeis.org/A330971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1422, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Incrementally check each integer starting from 0; keep those
   numbers that satisfy the property that for every decimal digit d,
   k mod (d+1) == 0.  The results are cached so that a(n) can be
   returned in O(1) after the first n terms have been discovered. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number satisfies the defining property. */
static inline bool check_valid(seq_t k)
{
    if (k == 0) return true;
    /* All multiples of 2520 satisfy the condition. */
    if (k % (seq_t)2520 == 0) return true;

    seq_t tmp = k;
    while (tmp != 0) {
        seq_t d   = tmp % 10;      /* current decimal digit */
        seq_t mod = d + 1;         /* divisor (d+1) */
        if (k % mod != 0) return false;
        tmp /= 10;
    }
    return true;
}

/* Return the nth term of sequence A330971 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    /* Static state to keep the computation across calls. */
    static seq_t *cache   = NULL; /* array of found terms   */
    static size_t cache_size = 0; /* number of stored terms */
    static size_t cache_cap  = 0; /* allocated capacity      */
    static seq_t current = 0;     /* next integer to test   */

    /* Compute terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        if (check_valid(current)) {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0;     /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = current;
        }
        current++;                 /* advance to the next integer */
    }

    return cache[n - 1];
}
