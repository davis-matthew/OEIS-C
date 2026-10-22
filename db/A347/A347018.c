/*
 * A347018  The number of unlabeled trees T on n vertices for which maximum
 *          multiplicity attained by any matrix whose graph is T implies the
 *          simplicity of its other eigenvalues.
 * https://oeis.org/A347018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=5, a(7)=8, a(8)=14,
 *            a(9)=24, a(10)=43, a(11)=74, a(12)=134, a(13)=238, a(14)=433,
 *            a(15)=778, for n>15:
 *            a(n)=2*a(n-1)+a(n-2)-3*a(n-3)+2*a(n-4)-a(n-5)-2*a(n-6)+a(n-7)
 *            +3*a(n-8)-4*a(n-9)-a(n-10)+2*a(n-11)-2*a(n-12)+2*a(n-13)+a(n-14)-a(n-15)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A347018: number of unlabeled NIM trees on n vertices. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache: 1‑indexed.  size_t indices are fine since n ≤ 10000 for timing. */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated size */
    static idx_t last = 0;     /* highest index computed */

    /* Initialise cache with base values up to 15 when first called. */
    if (!cache) {
        /* allocate a little more than 15 to avoid immediate re‑alloc on n=16 */
        cap = 32;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        /* base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)5;
        cache[7] = (seq_t)8;
        cache[8] = (seq_t)14;
        cache[9] = (seq_t)24;
        cache[10] = (seq_t)43;
        cache[11] = (seq_t)74;
        cache[12] = (seq_t)134;
        cache[13] = (seq_t)238;
        cache[14] = (seq_t)433;
        cache[15] = (seq_t)778;
        last = 15;
    }

    if (n == 0)
        return (seq_t)0;   /* undefined below offset, return 0 */

    /* Ensure capacity for n (array is 1‑indexed, so we need n+1 slots). */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (((size_t)n + 1) > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed, return 0 (unlikely) */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 15) {
            /* already initialised; nothing to do */
        } else {
            /* Compute using signed __int128 to avoid unsigned wrap‑around. */
            __int128 term = 0;
            term += (__int128)cache[i-1] * 2;
            term += (__int128)cache[i-2];
            term -= (__int128)cache[i-3] * 3;
            term += (__int128)cache[i-4] * 2;
            term -= (__int128)cache[i-5];
            term -= (__int128)cache[i-6] * 2;
            term += (__int128)cache[i-7];
            term += (__int128)cache[i-8] * 3;
            term -= (__int128)cache[i-9] * 4;
            term -= (__int128)cache[i-10];
            term += (__int128)cache[i-11] * 2;
            term -= (__int128)cache[i-12] * 2;
            term += (__int128)cache[i-13] * 2;
            term += (__int128)cache[i-14];
            term -= (__int128)cache[i-15];
            cache[i] = (seq_t)term;
        }
    }
    last = n;
    return cache[n];
}
