/*
 * A281062  Number of 7Xn 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) + 12*a(n-4) - 12*a(n-5) -
 *            3*a(n-6) + 6*a(n-7) - a(n-9) for n > 20 Initial terms: a(1)=0,
 *            a(2)=1686, a(3)=15028, a(4)=27635, a(5)=58005, a(6)=119205,
 *            a(7)=221212, a(8)=394884, a(9)=689397, a(10)=1191475,
 *            a(11)=2050466, a(12)=3523654, a(13)=6051629, a(14)=1038...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 25, u64 n <= 68, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t base[20] = {
        (seq_t)0, (seq_t)1686, (seq_t)15028, (seq_t)27635, (seq_t)58005,
        (seq_t)119205, (seq_t)221212, (seq_t)394884, (seq_t)689397,
        (seq_t)1191475, (seq_t)2050466, (seq_t)3523654, (seq_t)6051629,
        (seq_t)10389370, (seq_t)17827528, (seq_t)30568529, (seq_t)52364160,
        (seq_t)89595493, (seq_t)153099429, (seq_t)261254732
    };

    /* Return from base if within offset range */
    if (n <= (idx_t)20) {
        return base[n - 1];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of elements stored (a(1) ... a(cache_len)) */

    /* Initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = 20;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 20; ++i) {
            cache[i] = base[i];
        }
        cache_len = 20;
    }

    /* Grow capacity if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 20;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new terms up to n */
    if ((size_t)n > cache_len) {
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            size_t idx = (size_t)i;
            /* a(i-1) to a(i-9) */
            seq_t t1 = 6 * cache[idx - 2];   /* a(i-1) */
            seq_t t2 = 12 * cache[idx - 3];  /* a(i-2) */
            seq_t t3 = 5 * cache[idx - 4];   /* a(i-3) */
            seq_t t4 = 12 * cache[idx - 5];  /* a(i-4) */
            seq_t t5 = 12 * cache[idx - 6];  /* a(i-5) */
            seq_t t6 = 3 * cache[idx - 7];   /* a(i-6) */
            seq_t t7 = 6 * cache[idx - 8];   /* a(i-7) */
            seq_t t8 = cache[idx - 10];      /* a(i-9) */

            seq_t pos = t1 + t3 + t4 + t7;
            seq_t neg = t2 + t5 + t6 + t8;
            cache[idx - 1] = pos - neg;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
