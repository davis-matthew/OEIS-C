/*
 * A302189  Hurwitz inverse of squares [1,4,9,16,...].
 * https://oeis.org/A302189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (-1)^n - 3*n*a(n-1) - n*(n-1)*a(n-2)   (n ≥ 2), 
 * a(0) = 1, a(1) = -4
 */
#include <stdint.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;   /* memoisation array */
static size_t cap   = 0;      /* current allocated capacity */
static idx_t  last  = (idx_t)-1;   /* last computed index */

/* Return the n-th term of sequence A302189 */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise on first call */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        last = 0;
    }

    /* Compute terms up to n (any order of calls handled by memoisation) */
    while (last < n) {
        idx_t k = last + 1;          /* next index */

        /* (-1)^k */
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;

        /* 3*k*a(k-1)  (k ≥ 1) */
        seq_t term2 = (seq_t)3 * (seq_t)k * cache[k - 1];

        /* k*(k-1)*a(k-2)  (zero if k < 2) */
        seq_t term3 = 0;
        if (k >= 2) {
            term3 = (seq_t)k * (seq_t)(k - 1) * cache[k - 2];
        }

        /* a(k) = sign - term2 - term3 */
        cache[k] = sign - term2 - term3;
        last = k;
    }

    return cache[n];
}
