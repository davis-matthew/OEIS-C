/*
 * A291410  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S - S^2 - 2 S^3.
 * https://oeis.org/A291410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3) + 7*a(n-4) + 6*a(n-5) +
 *            2*a(n-6) for n >= 7 Initial values: a(0)=1, a(1)=3, a(2)=9,
 *            a(3)=26, a(4)=69, a(5)=186, a(6)=511 (All arithmetic is
 *            performed on `seq_t`, which may be an arbitrary‑precision type.)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static idx_t   last   = (idx_t)-1;   /* highest index computed so far */

    /* allocate space if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 7) new_cap = 7;          /* ensure room for first 7 terms */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* initialise the first 7 terms on first use */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)69;
        cache[5] = (seq_t)186;
        cache[6] = (seq_t)511;
        last    = 6;
    }

    /* if we already have n, simply return it */
    if (n <= last) return cache[(size_t)n];

    /* compute new terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 1)]
                  + (seq_t)2 * cache[(size_t)(i - 2)]
                  + (seq_t)4 * cache[(size_t)(i - 3)]
                  + (seq_t)7 * cache[(size_t)(i - 4)]
                  + (seq_t)6 * cache[(size_t)(i - 5)]
                  + (seq_t)2 * cache[(size_t)(i - 6)];
        cache[(size_t)i] = val;
    }
    last = n;
    return cache[(size_t)n];
}
