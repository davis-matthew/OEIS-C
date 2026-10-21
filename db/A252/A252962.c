/*
 * A252962  Number of (1+2)X(n+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for n > 16 * a(n) = a(n-1) + 9*a(n-2) -
 *            27*a(n-4) - 55*a(n-5) * + 46*a(n-6) +157*a(n-7) + 65*a(n-8) * -
 *            168*a(n-9) - 321*a(n-10) + 123*a(n-11) * + 171*a(n-12) +
 *            198*a(n-13) - 198*a(n-14) * * Initial values (1 ≤ n ≤ 16): *
 *            a(1)=60, a(2)=119, a(3)=223, a(4)=603, a(5)=1251, a(6)...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values (index 0 unused) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Hard‑coded initial segment (index 0 is dummy) */
static const seq_t init[17] = {
    0ULL,        /* dummy for index 0 */
    60ULL,       /* a(1) */
    119ULL,      /* a(2) */
    223ULL,      /* a(3) */
    603ULL,      /* a(4) */
    1251ULL,     /* a(5) */
    3465ULL,     /* a(6) */
    7495ULL,     /* a(7) */
    20977ULL,    /* a(8) */
    46641ULL,    /* a(9) */
    130201ULL,   /* a(10) */
    295421ULL,   /* a(11) */
    819671ULL,   /* a(12) */
    1887977ULL,  /* a(13) */
    5198935ULL,  /* a(14) */
    12122019ULL, /* a(15) */
    33114775ULL  /* a(16) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* safety, not part of the offset */

    /* Initialise cache with the 16 known entries if needed */
    if (cache_len < 16) {
        cache_len = 16;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= cache_len; ++i)
            cache[i] = init[i];
    }

    /* Extend cache to the requested index */
    if (cache_len < n) {
        idx_t old_len = cache_len;
        cache_len = n;
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));

        for (idx_t m = old_len + 1; m <= cache_len; ++m) {
            if (m <= 16) {                  /* keep the hard‑coded values */
                cache[m] = init[m];
                continue;
            }

            /* Positive contributions */
            seq_t pos = cache[m - 1];
            pos += cache[m - 2] * 9ULL;
            pos += cache[m - 6] * 46ULL;
            pos += cache[m - 7] * 157ULL;
            pos += cache[m - 8] * 65ULL;
            pos += cache[m - 11] * 123ULL;
            pos += cache[m - 12] * 171ULL;
            pos += cache[m - 13] * 198ULL;

            /* Negative contributions */
            seq_t neg = 0;
            neg += cache[m - 4] * 27ULL;
            neg += cache[m - 5] * 55ULL;
            neg += cache[m - 9] * 168ULL;
            neg += cache[m - 10] * 321ULL;
            neg += cache[m - 14] * 198ULL;

            cache[m] = pos - neg;          /* final result is guaranteed non‑negative */
        }
    }

    return cache[n];
}
