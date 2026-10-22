/*
 * A382793  a(n) = Sum_{k=0..n} (-1)^k * (Stirling2(n,k) * k!)^2.
 * https://oeis.org/A382793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (Stirling2(n,k) * k!)^2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* external integer types supplied by the build */
extern seq_t factorial(idx_t n);

/* internal cache for a(n) and the current row of B(n,k) = k! * S(n,k) */
static seq_t *a_cache = NULL;   /* cache of a(n) values         */
static seq_t *row    = NULL;    /* contents of B(n,k) row       */
static size_t cache_cap = 0;    /* current capacity of a_cache  */
static size_t row_cap    = 0;    /* current capacity of row      */
static idx_t   max_cached = 0;  /* largest n that has been cached */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) return 1;  /* a(0) = 1 */

    /* ensure enough space in a_cache */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – caller will not handle this */
        a_cache = tmp;
        /* zero newly allocated part */
        memset(a_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* ensure enough space in row */
    if (needed > row_cap) {
        size_t new_cap = row_cap ? row_cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(row, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        row = tmp;
        memset(row + row_cap, 0, (new_cap - row_cap) * sizeof(seq_t));
        row_cap = new_cap;
    }

    /* initialise for the very first call */
    if (max_cached == 0) {
        row[0] = 1;        /* B(0,0) = 1 */
        a_cache[0] = 1;    /* a(0)   = 1 */
    }

    /* compute rows up to n, storing each a(k) in the cache */
    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        /* update the row in place: go from cur down to 1 */
        for (idx_t k = cur; k >= 1; --k) {
            seq_t sum = row[k] + row[k - 1];
            row[k] = sum * (seq_t)k;            /* B(cur,k) */
        }
        row[0] = 0;                              /* B(cur,0) = 0 for cur>0 */

        /* compute a(cur) as alternating sum of squares of the row */
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t sq = row[k] * row[k];
            if (k & 1)   /* odd k → (-1)^k = -1 */
                sum -= sq;
            else
                sum += sq;
        }
        a_cache[cur] = sum;
    }

    max_cached = n;
    return a_cache[n];
}
