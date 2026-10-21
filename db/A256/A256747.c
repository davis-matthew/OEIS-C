/*
 * A256747  Number of (n+2)X(7+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 3 and no antidiagonal sum 3 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A256747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-1) +2*a(n-2) +12*a(n-3) +17*a(n-4) - 6*a(n-5)
 *            -2*a(n-6) +4*a(n-7) -144*a(n-8) - 304*a(n-9) -96*a(n-10)
 *            +512*a(n-11) +512*a(n-12) for n > 15; Initial values:
 *            a(1)=31032, a(2)=28816, a(3)=38376, a(4)=33616, a(5)=96576,
 *            a(6)=155792, a(7)=404480, a(8)=615296, a(9)=1258112,
 *            a(10)=1664400...
 * Range:     u8 none, u16 n <= 4, u32 n <= 21, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n = 0 */

    /* Store the first 15 values */
    static const seq_t init[15] = {
        (seq_t)31032,   (seq_t)28816,   (seq_t)38376,   (seq_t)33616,
        (seq_t)96576,   (seq_t)155792,  (seq_t)404480,  (seq_t)615296,
        (seq_t)1258112, (seq_t)1664400, (seq_t)5004928, (seq_t)7111808,
        (seq_t)19624576,(seq_t)26821008,(seq_t)65198848
    };

    /* Memoisation cache: 1‑based index → cache[n-1] = a(n) */
    static seq_t *cache   = NULL;
    static idx_t stored   = 0;   /* number of terms currently stored */
    static size_t capacity = 0;  /* allocated capacity of cache */

    /* Initialise cache on first call */
    if (!cache) {
        capacity = 15;
        cache    = alloc_seq_array(capacity);
        for (idx_t i = 0; i < 15; ++i) cache[i] = init[i];
        stored   = 15;
    }

    /* If requested term already cached, return it */
    if (n <= stored) return cache[n-1];

    /* Grow cache to accommodate the requested index */
    if (n > capacity) {
        grow_seq_array(&cache, &capacity, (size_t)n);
    }

    /* Compute terms beyond those already stored, using the recurrence */
    for (idx_t idx = stored; idx < n; ++idx) {
        /* Positive contributions (coefficients > 0) */
        seq_t sum = 0;
        sum += cache[idx-2] * (seq_t)2;
        sum += cache[idx-3] * (seq_t)12;
        sum += cache[idx-4] * (seq_t)17;
        sum += cache[idx-7] * (seq_t)4;
        sum += cache[idx-11] * (seq_t)512;
        sum += cache[idx-12] * (seq_t)512;

        /* Negative contributions (coefficients < 0) */
        seq_t neg = 0;
        neg += cache[idx-1] * (seq_t)2;
        neg += cache[idx-5] * (seq_t)6;
        neg += cache[idx-6] * (seq_t)2;
        neg += cache[idx-8] * (seq_t)144;
        neg += cache[idx-9] * (seq_t)304;
        neg += cache[idx-10] * (seq_t)96;

        sum -= neg;
        cache[idx] = sum;
    }

    stored = n;            /* all terms up to n are now cached */
    return cache[n-1];
}
