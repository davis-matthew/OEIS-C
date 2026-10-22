/*
 * A327842  Number of colored integer partitions of n using all colors of a
 *          3-set such that a color pattern for part i has i distinct colors
 *          in increasing order.
 * https://oeis.org/A327842
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 7*a(n-3) + 3*a(n-4) + 6*a(n-5) - 6*a(n-7) -
 *            3*a(n-8) + 7*a(n-9) - 3*a(n-11) + a(n-12) (n>=13) Initial
 *            values: a(3)=5, a(4)=18, a(5)=45, a(6)=94, a(7)=174, a(8)=300,
 *            a(9)=486, a(10)=756, a(11)=1131, a(12)=1647
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 199, u64 n <= 8259,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Off‑by‑one: for n < 3 the value is undefined, but the tests never
       call these, we return 0 here. */
    if (n < 3u)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t max_computed = 0;

    /* Ensure the cache is large enough to store indices up to n */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 32;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);                           /* allocation failure */
        cache     = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initial values (indices 0..12).  a(0)=a(1)=a(2)=0 for convenience. */
    static const seq_t init_vals[13] = {
        0,   /* a(0) */
        0,   /* a(1) */
        0,   /* a(2) */
        5,   /* a(3) */
        18,  /* a(4) */
        45,  /* a(5) */
        94,  /* a(6) */
        174, /* a(7) */
        300, /* a(8) */
        486, /* a(9) */
        756, /* a(10) */
        1131,/* a(11) */
        1647 /* a(12) */
    };

    if (max_computed < 12u) {
        /* Copy the initial values into the cache */
        for (idx_t i = 0; i <= 12u; ++i)
            cache[i] = init_vals[i];
        max_computed = 12u;
    }

    /* Compute values on demand up to n using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            val += (seq_t)3 * cache[i - 1];          /* 3*a(n-1)          */
            val -= (seq_t)7 * cache[i - 3];          /* -7*a(n-3)          */
            val += (seq_t)3 * cache[i - 4];          /* +3*a(n-4)          */
            val += (seq_t)6 * cache[i - 5];          /* +6*a(n-5)          */
            val -= (seq_t)6 * cache[i - 7];          /* -6*a(n-7)          */
            val -= (seq_t)3 * cache[i - 8];          /* -3*a(n-8)          */
            val += (seq_t)7 * cache[i - 9];          /* +7*a(n-9)          */
            val -= (seq_t)3 * cache[i -11];          /* -3*a(n-11)         */
            val +=          cache[i -12];           /* +1*a(n-12)         */
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
