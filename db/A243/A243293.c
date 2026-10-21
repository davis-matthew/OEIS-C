/*
 * A243293  Number of factorials < 10^n.
 * https://oeis.org/A243293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max k such that k! < 10^n (counting distinct factorials,
 *            0! and 1! count once)
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy static array holding cumulative log10(k!) for k = 0,1,2,... */
    static double *cum_log = NULL;          /* cumulative log10(k!) */
    static size_t capacity = 0;             /* allocated size */
    static size_t len = 0;                  /* number of entries currently stored (len-1 is last index) */

    /* initialise with k=0 and k=1 (both have log10 = 0) */
    if (cum_log == NULL) {
        capacity = 4;
        cum_log = (double *)malloc(capacity * sizeof(double));
        if (!cum_log) return (seq_t)0;      /* allocation failed, return 0 (unlikely) */
        cum_log[0] = 0.0;                   /* log10(0!) = 0  */
        cum_log[1] = 0.0;                   /* log10(1!) = 0  */
        len = 2;                            /* valid indices: 0 .. 1 */
    }

    if (n < 1)
        return (seq_t)0;                     /* sequence is defined for n >= 1 */

    /* extend the array until the last cumulative log exceeds or equals n */
    while (cum_log[len - 1] < (double)n) {
        if (len == capacity) {
            size_t newcap = capacity * 2;
            double *tmp = (double *)realloc(cum_log, newcap * sizeof(double));
            if (!tmp) return (seq_t)0;      /* allocation failed */
            cum_log = tmp;
            capacity = newcap;
        }
        /* compute log10((len)!) = log10((len-1)!) + log10(len) */
        cum_log[len] = cum_log[len - 1] + log10((double)len);
        len++;
    }

    /* binary search for the largest k with cum_log[k] < n */
    size_t lo = 1;                   /* for n >= 1, answer >= 1 */
    size_t hi = len - 1;
    size_t ans = 1;

    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cum_log[mid] < (double)n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;    /* safety, should not happen */
            hi = mid - 1;
        }
    }
    return (seq_t)ans;
}
