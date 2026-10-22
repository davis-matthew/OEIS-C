/*
 * A398484  Triangle read by rows, a generalization of Ramanujan's Q function.
 *          T(n, k) = numerator(Q(n, k)), where Q(n, k) = Sum_{i=0..n-k}
 *          falling_factorial(n - k, i) / n^i.
 * https://oeis.org/A398484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A398484
 *
 *  Triangle T(n,k) (0≤k≤n) defined by
 *      Q(n,k) = 1 + Q(n,k+1)·(n-k)/n   for k < n,
 *      Q(n,n) = 1
 *  where Q(n,k) is a rational number.
 *  T(n,k) is the numerator of Q(n,k) in lowest terms.
 *
 *  The sequence a(n) is the triangle read by rows:
 *      a(0)   = T(0,0)
 *      a(1)   = T(1,0), a(2) = T(1,1)
 *      a(3)   = T(2,0), a(4) = T(2,1), a(5) = T(2,2)
 *      ...
 *
 *  This implementation builds each row once, keeps the numerators
 *  in a cache, and on demand returns the value at index n.
 *
 *  All arithmetic is performed in the seq_t type, which may be a
 *  built‑in unsigned integer or an arbitrary‑precision integer class.
 *
 *  The algorithm uses only addition, subtraction, multiplication,
 *  and gcd.  No overflow occurs in builds where seq_t is an
 *  arbitrary‑precision type, and for 64‑bit builds the intermediate
 *  values stay below 2^64 for all rows whose final value fits in
 *  64 bits.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/gcd.h"            /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* static cache of computed triangle values in row‑major order */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static size_t cache_len  = 0;   /* number of values stored */
    static idx_t  next_row   = 0;   /* next row to be generated */

    /* Generate rows until we have at least n+1 values. */
    while (cache_len <= n) {
        idx_t r = next_row;                     /* current row number (n) */

        /* Ensure capacity for the new row (r+1 entries). */
        size_t base = cache_len;                 /* start index for this row */
        size_t needed = base + (size_t)(r + 1);
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < needed) new_cap = needed;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) {
                free(cache);
                abort();
            }
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Recurrence: start from Q(r,r)=1 (numerator 1, denominator 1). */
        seq_t num = (seq_t)1;
        seq_t den = (seq_t)1;

        /* Compute columns k=r down to 0. */
        for (idx_t k = r; ; --k) {
            /* Store the current numerator at the correct offset. */
            cache[base + k] = num;

            /* If we have reached the first column, finish. */
            if (k == 0) break;

            /* Compute next Q(r,k-1):
             * new_num = num * (r-k+1) + den * r
             * new_den = den * r
             */
            idx_t m = r - k + 1;                 /* m = n - k + 1   */
            seq_t new_num = num * (seq_t)m + den * (seq_t)r;
            seq_t new_den = den * (seq_t)r;

            /* Reduce fraction. */
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }

        /* Update state for next generation. */
        cache_len += (size_t)(r + 1);
        next_row++;
    }

    return cache[n];
}
