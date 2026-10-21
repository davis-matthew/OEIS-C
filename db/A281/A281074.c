/*
 * A281074  Number of n X 2 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(1)=0, a(2)=0, a(3)=6, a(4)=24, a(5)=92, a(6)=318, a(7)=1056,
   a(8)=3406, a(9)=10770, a(10)=33542, a(11)=103226, a(12)=314634.
   For n>12:
   a(n) = 6*a(n-1) - 11*a(n-2) + 8*a(n-3) - 5*a(n-4) - 8*a(n-5)
          + 11*a(n-6) + a(n-8) + 6*a(n-9) - 2*a(n-10) - 4*a(n-11) - a(n-12).
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Undefined for n<1, return 0 */

    /* Static cache for memoization */
    static seq_t *cache      = NULL;        /* array of computed values, 1‑indexed */
    static size_t cache_cap  = 0;           /* current capacity of cache */
    static size_t cache_used = 0;           /* next free index (first unused) */

    /* Ensure cache has enough capacity for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        /* If this is the first allocation, initialize base cases */
        if (cache_used == 0) {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)6;
            cache[4] = (seq_t)24;
            cache[5] = (seq_t)92;
            cache[6] = (seq_t)318;
            cache[7] = (seq_t)1056;
            cache[8] = (seq_t)3406;
            cache[9] = (seq_t)10770;
            cache[10] = (seq_t)33542;
            cache[11] = (seq_t)103226;
            cache[12] = (seq_t)314634;
            cache_used = 13; /* indices 1..12 are now defined */
        }
    }

    /* If the requested term was already computed, return it */
    if (n < (idx_t)cache_used)
        return cache[n];

    /* Compute missing terms up to n (including n) */
    for (idx_t i = (idx_t)cache_used; i <= n; ++i) {
        seq_t val = (seq_t)0;
        seq_t tmp;

        /* 6* a(i-1) */
        tmp = cache[i-1];
        tmp *= (seq_t)6;
        val += tmp;

        /* -11* a(i-2) */
        tmp = cache[i-2];
        tmp *= (seq_t)11;
        val -= tmp;

        /* +8* a(i-3) */
        tmp = cache[i-3];
        tmp *= (seq_t)8;
        val += tmp;

        /* -5* a(i-4) */
        tmp = cache[i-4];
        tmp *= (seq_t)5;
        val -= tmp;

        /* -8* a(i-5) */
        tmp = cache[i-5];
        tmp *= (seq_t)8;
        val -= tmp;

        /* +11* a(i-6) */
        tmp = cache[i-6];
        tmp *= (seq_t)11;
        val += tmp;

        /* +1* a(i-8) */
        val += cache[i-8];

        /* +6* a(i-9) */
        tmp = cache[i-9];
        tmp *= (seq_t)6;
        val += tmp;

        /* -2* a(i-10) */
        tmp = cache[i-10];
        tmp *= (seq_t)2;
        val -= tmp;

        /* -4* a(i-11) */
        tmp = cache[i-11];
        tmp *= (seq_t)4;
        val -= tmp;

        /* -1* a(i-12) */
        val -= cache[i-12];

        cache[i] = val;
    }

    cache_used = (size_t)(n + 1); /* now we have terms up to n */
    return cache[n];
}
