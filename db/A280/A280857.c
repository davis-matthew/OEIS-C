/*
 * A280857  Number of nX6 0..2 arrays with no element equal to more than one
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A280857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) +
 *            6*a(n-7) -5*a(n-8) +3*a(n-9) -3*a(n-10) -3*a(n-11) +2*a(n-12)
 *            -4*a(n-13) +a(n-14) -2*a(n-15) -2*a(n-16) Initial values (for 1
 *            ≤ n ≤ 22): a(1)=82, a(2)=1708, a(3)=678, a(4)=462, a(5)=491,
 *            a(6)=712, a(7)=1202, a(8)=2268, a(9)=4220, a(1...
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The cache holds terms a(1) .. a(L).  Index 0 in the array
       corresponds to a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initial base values (first 22 terms). */
    static const seq_t base[22] = {
        (seq_t)82,     (seq_t)1708,   (seq_t)678,    (seq_t)462,
        (seq_t)491,    (seq_t)712,    (seq_t)1202,   (seq_t)2268,
        (seq_t)4220,   (seq_t)7108,   (seq_t)11240,  (seq_t)17330,
        (seq_t)26934,  (seq_t)43062,  (seq_t)70776,  (seq_t)117452,
        (seq_t)193842, (seq_t)316394, (seq_t)511776, (seq_t)825074,
        (seq_t)1333130,(seq_t)2163076
    };

    /* First time: allocate cache and copy base values. */
    if (cache_len == 0) {
        cache_cap = 64;                         /* initial capacity > 22 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 22; ++i)
            cache[i] = base[i];
        cache_len = 22;
    }

    /* Extend cache up to required index n. */
    if (n > cache_len) {
        /* Ensure capacity. */
        if (n > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 22) {
                /* already present from base */
                continue;
            }
            /* Recurrence for i > 22.  All indices below refer to
               entries already computed. */
            seq_t pos = cache[i-2]            /* a(i-1)  */
                     + cache[i-3]            /* a(i-2)  */
                     + cache[i-5]            /* a(i-4)  */
                     + (seq_t)6 * cache[i-8] /* 6*a(i-7) */
                     + (seq_t)3 * cache[i-10]/* 3*a(i-9) */
                     + (seq_t)2 * cache[i-13]/* 2*a(i-12) */
                     + cache[i-15];         /* a(i-14) */

            seq_t neg = cache[i-4]            /* a(i-3)  */
                     + (seq_t)5 * cache[i-9] /* 5*a(i-8) */
                     + (seq_t)3 * cache[i-11]/* 3*a(i-10) */
                     + (seq_t)3 * cache[i-12]/* 3*a(i-11) */
                     + (seq_t)4 * cache[i-14]/* 4*a(i-13) */
                     + (seq_t)2 * cache[i-16]/* 2*a(i-15) */
                     + (seq_t)2 * cache[i-17];/* 2*a(i-16) */

            cache[i-1] = pos - neg;  /* pos > neg for all valid i */
        }
        cache_len = n;
    }

    return cache[n-1];
}
