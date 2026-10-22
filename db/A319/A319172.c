/*
 * A319172  a(n) = 2*(a(n-1) + a(n-3)) - a(n-4), with a(0) = 1, a(1) = 2, a(2)
 *          = 5 and a(3) = 12.
 * https://oeis.org/A319172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(a(n-1)+a(n-3)) - a(n-4), with a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=12
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache the sequence up to the largest n that has been computed.
 * The cache is grown only when necessary; it never shrinks.
 * All indices are zero‑based and stored in a 32/64‑bit unsigned integer
 * whose type depends on the build.  The values are stored in type
 * seq_t, which may be an unsigned 8/16/32/64/128 bit integer or an
 * arbitrary‑precision integer type.  The arithmetic that appears
 * uses only the C operators allowed by the problem statement. */
seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;

    /* Ensure the cache is large enough to hold up to n (inclusive). */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap < need)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);                /* allocation failed – abort safely */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first four terms once. */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)12;  /* a(3) */
        len     = 4;
    }

    /* Compute any missing terms from len up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = (cache[i - 1] + cache[i - 3]) * (seq_t)2 - cache[i - 4];
        cache[i] = val;
    }

    /* Update cache length to the largest index that has been filled. */
    if (len <= (size_t)n)
        len = (size_t)n + 1;

    /* Return the requested term. */
    return cache[(size_t)n];
}
