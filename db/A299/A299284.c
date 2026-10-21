/*
 * A299284  Partial sums of A299283.
 * https://oeis.org/A299284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A299284 – partial sums of A299283
   Recurrence (n > 6):
          a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7)
   Initial values:
          a(0)=1, a(1)=8, a(2)=30, a(3)=78, a(4)=162, a(5)=292, a(6)=478, a(7)=731
   The function is memoised but works for calls in any order. */

#include <stdlib.h>

/* ------------------------------------------------------------ */
static seq_t *cache = NULL;   /* memoisation array          */
static size_t  size   = 0;    /* number of cached elements   */
static size_t  cap    = 0;    /* current allocation capacity */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise first eight terms */
    if (size == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)78;
        cache[4] = (seq_t)162;
        cache[5] = (seq_t)292;
        cache[6] = (seq_t)478;
        cache[7] = (seq_t)731;
        size = cap;
    }

    /* grow cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > size) {
        if (need > cap) {
            size_t new_cap = cap ? cap : 8;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure – unlikely */
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = size; i < need; ++i) {
            long long val =
                (long long)3 * (long long)cache[i-1]   /* 3*a(i-1)  */
              - (long long)3 * (long long)cache[i-2]   /* -3*a(i-2) */
              + (long long)cache[i-3]                 /* +a(i-3)   */
              + (long long)cache[i-4]                 /* +a(i-4)   */
              - (long long)3 * (long long)cache[i-5]   /* -3*a(i-5) */
              + (long long)3 * (long long)cache[i-6]   /* +3*a(i-6) */
              - (long long)cache[i-7];                /* -a(i-7)   */
            cache[i] = (seq_t)val;
        }
        size = need;
    }

    return cache[(size_t)n];
}
