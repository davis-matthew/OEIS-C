/*
 * A245956  Number of length 6+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245956
   Recurrence:
   a(n) = 3*a(n-1) + 2*a(n-2) -14*a(n-3) + 5*a( n-4) + 25*a(n-5)
          -20*a(n-6) -20*a(n-7) +25*a(n-8) +5*a(n-9)
          -14*a(n-10)+ 2*a(n-11)+ 3*a(n-12)-a(n-13)
   initial values:
     a(1) = 298,  a(2) = 11441,  a(3) = 88804,  a(4) = 525041,
     a(5) = 1744494, a(6) = 5208673, a(7) = 12257032,
     a(8) = 27206945, a(9) = 53051890, a(10) = 99643601,
     a(11) = 172531308, a(12) = 290962321, a(13) = 464076214
*/

#include <stddef.h>
#include <stdbool.h>

/* up to about 20000 terms are required for the test harness */
#define MAX_TERMS 20000

static seq_t cache[MAX_TERMS];
static idx_t cache_last;          /* number of terms already computed (a(1)->cache[0]) */
static bool initialized = false;

/* 64‑bit constants of the first 13 terms – cast to seq_t at run time */
static const uint64_t init_vals64[13] = {
    298ULL, 11441ULL, 88804ULL, 525041ULL, 1744494ULL,
    5208673ULL, 12257032ULL, 27206945ULL, 53051890ULL,
    99643601ULL, 172531308ULL, 290962321ULL, 464076214ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined – return 0 for safety */

    if (!initialized) {
        /* first time: fill the cache with the 13 known terms */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = (seq_t)init_vals64[i];
        cache_last = 13;          /* a(13) is stored at cache[12] */
        initialized = true;
    }

    if (n <= cache_last)
        return cache[n - 1];       /* already computed */

    if (n > MAX_TERMS)
        return 0;                 /* out of supported range */

    /* iterate from cache_last+1 up to n, using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        idx_t idx = i - 1;                 /* zero based index for a(i) */
        /* recurrence uses a(i-1) .. a(i-13) which are cache[idx-1] .. cache[idx-13] */
        seq_t sum = 0;
        sum += cache[idx - 1] * (seq_t)3;
        sum += cache[idx - 2] * (seq_t)2;
        sum -= cache[idx - 3] * (seq_t)14;
        sum += cache[idx - 4] * (seq_t)5;
        sum += cache[idx - 5] * (seq_t)25;
        sum -= cache[idx - 6] * (seq_t)20;
        sum -= cache[idx - 7] * (seq_t)20;
        sum += cache[idx - 8] * (seq_t)25;
        sum += cache[idx - 9] * (seq_t)5;
        sum -= cache[idx -10] * (seq_t)14;
        sum += cache[idx -11] * (seq_t)2;
        sum += cache[idx -12] * (seq_t)3;
        sum -= cache[idx -13];
        cache[idx] = sum;           /* store computed a(i) */
    }
    cache_last = n;
    return cache[n - 1];
}
