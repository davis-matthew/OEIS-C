/*
 * A309229  Square array read by upwards antidiagonals: T(n,k) = Sum_{i=1..n}
 *          A191898(i,k).
 * https://oeis.org/A309229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A309229
    Formula (recurrence for the generating array T(n,k)):
          T(n,1)   = n                (n ≥ 1)
          T(1,k)   = 1                (k ≥ 1)
          T(n,k)   = (n>k) ? T(n-k,k) :
                          Σ[i=0..n-1] T(n-1,k-i) – Σ[i=1..n-1] T(n,k-i)
    The sequence a(n) is the array read by upward antidiagonals:
          let d be the least integer with d(d-1)/2 ≥ n
          let prev = (d-1)(d-2)/2
          let offset = n - prev   (1 ≤ offset ≤ d-1)
          then a(n) = T(d-offset, offset)
    The routine below implements this definition with memoisation
    for the small indices that occur for the usual values of n
    (up to about 10 000 terms the indices never exceed 2 048 × 2 048).
*/

#include <stdint.h>
#include <stdlib.h>

/* cache size – large enough for all indices that occur
   for n <= 10 000; can be increased if needed                 */
#define CACHE_MAX 2048

/* global cache for computed T(n,k) values                                 */
static seq_t             t_cache[CACHE_MAX + 1][CACHE_MAX + 1];
static unsigned char     t_cached[CACHE_MAX + 1][CACHE_MAX + 1];

/* --------------------------------------------------------------------- */
/*  helper: triangular number d*(d-1)/2, performed in 128‑bit arithmetic  */
static inline idx_t tri(idx_t d)
{
    return (idx_t)(((__uint128_t)d * (d - (idx_t)1)) >> 1);
}

/* --------------------------------------------------------------------- */
/*  helper: compute the diagonal index d that satisfies tri(d) ≥ n         */
static idx_t calc_diagonal(idx_t n)
{
    idx_t low = 1, high = 2;
    /* increase high until it is large enough                                */
    while (tri(high) < n)
        high <<= 1;
    /* binary search for the smallest d with tri(d) ≥ n                       */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/*  recursive computation of T(n,k) with memoisation for small indices   */
static seq_t Tfunc(idx_t n, idx_t k)
{
    /* base cases */
    if (n == (idx_t)1)
        return (seq_t)1;               /* T(1,k) = 1   */
    if (k == (idx_t)1)
        return (seq_t)n;               /* T(n,1) = n   */

    /* use cache only for indices that fit in the array                */
    if (n <= CACHE_MAX && k <= CACHE_MAX) {
        if (t_cached[n][k])
            return t_cache[n][k];
    }

    seq_t result;
    if (n > k) {
        result = Tfunc(n - k, k);
    } else {
        seq_t sum1 = 0;
        for (idx_t i = 0; i < n; ++i)
            sum1 += Tfunc(n - 1, k - i);
        seq_t sum2 = 0;
        for (idx_t i = 1; i < n; ++i)
            sum2 += Tfunc(n, k - i);
        result = sum1 - sum2;
    }

    /* store in cache if we can                                  */
    if (n <= CACHE_MAX && k <= CACHE_MAX) {
        t_cached[n][k] = 1;
        t_cache[n][k] = result;
    }

    return result;
}

/* --------------------------------------------------------------------- */
/*  public entry point                                                    */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* only defined for n ≥ 1, return 0 for 0 */
        return (seq_t)0;

    idx_t d      = calc_diagonal(n);
    idx_t prev   = tri(d - 1);
    idx_t offset = n - prev;               /* 1‑based offset in this diagonal   */
    idx_t row    = d - offset;              /* row index (≥1)                      */
    idx_t col    = offset;                  /* column index  (≥1)                  */
    return Tfunc(row, col);
}
