/*
 * A250806  Number of (n+1) X (2+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n>=4 Base: a(1)=100,
 *            a(2)=379, a(3)=1315 (Equivalent closed form: a(n) = (126*3^n -
 *            99*2^n + 20)/2)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The following static variables keep a memoization table so that
   calls to a() in any order remain fast. */
static seq_t *cache = NULL;      /* array of cached values */
static idx_t  cache_len = 0;     /* number of terms stored (a(1) is cache[0]) */
static size_t cache_cap = 0;     /* current allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe guard */

    /* Initialise cache on first call.  Store the first three terms. */
    if (cache == NULL) {
        cache_cap = 4; /* small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)100;   /* a(1) */
        cache[1] = (seq_t)379;   /* a(2) */
        cache[2] = (seq_t)1315;  /* a(3) */
        cache_len = 3;
    }

    /* Grow the cache until it contains a(n).  Use the recurrence. */
    while (cache_len < n) {
        seq_t a_nm1 = cache[cache_len - 1]; /* a(k-1) where k = cache_len+1 */
        seq_t a_nm2 = cache[cache_len - 2]; /* a(k-2) */
        seq_t a_nm3 = cache[cache_len - 3]; /* a(k-3) */

        /* next = 6*a(k-1) - 11*a(k-2) + 6*a(k-3) */
        seq_t next = (seq_t)6 * a_nm1 + (seq_t)6 * a_nm3 - (seq_t)11 * a_nm2;

        /* Ensure we have room for the new term. */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
