/*
 * A260100  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000101 or 00010101.
 * https://oeis.org/A260100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A260100
    a(1)=85, a(2)=201, a(3)=477, a(4)=1151,
    a(5)=2794, a(6)=6804, a(7)=16453, a(8)=39854,
    a(9)=96501, a(10)=233975, a(11)=566755, a(12)=1373344,
    a(13)=3326942, a(14)=8061337, a(15)=19530608, a(16)=47321405,
    a(17)=114650673, a(18)=277784695.
    For n>18:
    a(n)=a(n-1)+2*a(n-2)+12*a(n-4)-a(n-5)-8*a(n-6)+2*a(n-7)
          -51*a(n-8)-22*a(n-9)-12*a(n-10)-46*a(n-11)
          +31*a(n-12)+12*a(n-13)-21*a(n-14)+18*a(n-15)
          -6*a(n-16)-18*a(n-17). */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* for uintptr_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    /* memoisation state */
    static seq_t *cache      = NULL;   /* array index i gives a(i) */
    static size_t cache_cap  = 0;      /* allocated size (>=1) */
    static size_t last_computed = 0;   /* largest n with a(n) stored */
    static int    init_done     = 0;    /* 0: initial values not yet set */

    /* Ensure capacity at least n+1 (indices 0..n) */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache      = tmp;
        cache_cap  = needed;
    }

    /* --- initialise first 18 terms once --- */
    if (!init_done) {
        /* we need indices 1..18, so at least 19 slots */
        const size_t init_cap_needed = 19;   /* slots 0..18 */
        if (cache_cap < init_cap_needed) {
            seq_t *tmp = (seq_t *)realloc(cache, init_cap_needed * sizeof(seq_t));
            if (!tmp) return 0;
            cache      = tmp;
            cache_cap  = init_cap_needed;
        }

        /* fixed initial values */
        static const seq_t init_vals[18] = {
            85ULL, 201ULL, 477ULL, 1151ULL, 2794ULL, 6804ULL,
            16453ULL, 39854ULL, 96501ULL, 233975ULL, 566755ULL,
            1373344ULL, 3326942ULL, 8061337ULL, 19530608ULL,
            47321405ULL, 114650673ULL, 277784695ULL
        };
        for (size_t i = 1; i <= 18; ++i)
            cache[i] = init_vals[i-1];
        last_computed = 18;
        init_done = 1;
    }

    /* If asked for already known value, return it */
    if ((size_t)n <= last_computed)
        return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)last_computed + 1; i <= n; ++i) {
        /* indices used in recurrence are positive for i>=19 */
        size_t ii = (size_t)i;

        seq_t sum = cache[ii-1];                 /* a(i-1) */
        sum += 2 * cache[ii-2];                 /* +2*a(i-2) */
        sum += 12 * cache[ii-4];                /* +12*a(i-4) */
        sum += 2 * cache[ii-7];                 /* +2*a(i-7) */
        sum += 31 * cache[ii-12];               /* +31*a(i-12) */
        sum += 12 * cache[ii-13];               /* +12*a(i-13) */
        sum += 18 * cache[ii-15];               /* +18*a(i-15) */

        sum -= cache[ii-5];                     /* -a(i-5) */
        sum -= 8 * cache[ii-6];                 /* -8*a(i-6) */
        sum -= 51 * cache[ii-8];                /* -51*a(i-8) */
        sum -= 22 * cache[ii-9];                /* -22*a(i-9) */
        sum -= 12 * cache[ii-10];               /* -12*a(i-10) */
        sum -= 46 * cache[ii-11];               /* -46*a(i-11) */
        sum -= 21 * cache[ii-14];               /* -21*a(i-14) */
        sum -= 6 * cache[ii-16];                /* -6*a(i-16) */
        sum -= 18 * cache[ii-17];               /* -18*a(i-17) */

        cache[ii] = sum;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
