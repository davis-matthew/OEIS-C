/*
 * A264537  Number of (4+1) X (n+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change -2,-2 -1,0 0,1 or 1,0.
 * https://oeis.org/A264537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 3*a(n-1) + 18*a(n-3) + 120*a(n-5) - 24*a(n-6) + 288*a(n-7) + 256*a(n-10) for n > 11
   with initial values (for 1 ≤ n ≤ 11):
   a(1)=0, a(2)=19, a(3)=40, a(4)=166, a(5)=824, a(6)=3256,
   a(7)=15028, a(8)=64156, a(9)=275764, a(10)=1203956, a(11)=5186196 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for dynamic programming; index 0 unused */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;          /* allocated capacity (>= max_computed + 1) */
    static idx_t   max   = 0;          /* largest n computed so far */

    /* Return 0 for n < 1 (undefined in OEIS, but safe fallback) */
    if (n < 1) return 0;

    /* Initial allocation with the first 11 values */
    if (!cache) {
        cap = 12;        /* indices 0..11 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */

        /* set initial values for n = 1..11 */
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)19;
        cache[3]  = (seq_t)40;
        cache[4]  = (seq_t)166;
        cache[5]  = (seq_t)824;
        cache[6]  = (seq_t)3256;
        cache[7]  = (seq_t)15028;
        cache[8]  = (seq_t)64156;
        cache[9]  = (seq_t)275764;
        cache[10] = (seq_t)1203956;
        cache[11] = (seq_t)5186196;
        max = 11;
    }

    /* Ensure cache can hold indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* If value already computed, return it immediately */
    if (n <= max) return cache[n];

    /* Compute values from max+1 up to n using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* 3*a(i-1) */
        sum += (seq_t)3 * cache[i - 1];
        /* 18*a(i-3) */
        if (i >= 3) sum += (seq_t)18 * cache[i - 3];
        /* 120*a(i-5) */
        if (i >= 5) sum += (seq_t)120 * cache[i - 5];
        /* -24*a(i-6) */
        if (i >= 6) sum -= (seq_t)24 * cache[i - 6];
        /* 288*a(i-7) */
        if (i >= 7) sum += (seq_t)288 * cache[i - 7];
        /* 256*a(i-10) */
        if (i >= 10) sum += (seq_t)256 * cache[i - 10];

        cache[i] = sum;   /* works modulo 2^N for native integer types,
                             and arbitrary precision for big‑int types */
    }

    max = n;
    return cache[n];
}
