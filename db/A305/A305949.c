/*
 * A305949  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305949
   Linear recurrence (n>17):
     a(n) = 5*a(n-1) +7*a(n-2) -13*a(n-3) -88*a(n-4) -25*a(n-5)
             +177*a(n-6) +214*a(n-7) +73*a(n-8)
             -188*a(n-9) -194*a(n-10) -92*a(n-11)
             -26*a(n-12) -26*a(n-13) -18*a(n-14)
             +16*a(n-15) +8*a(n-16)
   Initial values (n=1..17):
     1, 8, 23, 107, 537, 2800, 14652, 77128, 406622,
     2146188, 11333438, 59863786, 316248100, 1670789296,
     8827362224, 46638933714, 246416884814.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialise cache with first 17 terms once */
    if (!cache) {
        cache_cap = 64;              /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        static const seq_t init[17] = {
            1ULL, 8ULL, 23ULL, 107ULL, 537ULL, 2800ULL,
            14652ULL, 77128ULL, 406622ULL, 2146188ULL,
            11333438ULL, 59863786ULL, 316248100ULL,
            1670789296ULL, 8827362224ULL, 46638933714ULL,
            246416884814ULL
        };
        for (idx_t i = 0; i < 17; ++i) cache[i] = init[i];
        cache_len = 17;
    }

    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 0;
        pos += 5  * cache[i - 2];
        pos += 7  * cache[i - 3];
        pos += 177 * cache[i - 7];
        pos += 214 * cache[i - 8];
        pos += 73  * cache[i - 9];
        pos += 16 * cache[i - 16];
        pos += 8  * cache[i - 17];

        /* Negative contributions */
        seq_t neg = 0;
        neg += 13 * cache[i - 4];
        neg += 88 * cache[i - 5];
        neg += 25 * cache[i - 6];
        neg += 188 * cache[i - 10];
        neg += 194 * cache[i - 11];
        neg += 92  * cache[i - 12];
        neg += 26  * cache[i - 13];
        neg += 26  * cache[i - 14];
        neg += 18  * cache[i - 15];

        cache[i - 1] = pos - neg;   /* pos >= neg for all n */
    }

    cache_len = n;
    return cache[n - 1];
}
