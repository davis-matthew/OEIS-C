/*
 * A268052  Number of n X 4 0..2 arrays with new values introduced in each row
 *          and column in sequential order starting with zero.
 * https://oeis.org/A268052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 108*a(n-2) + 140*a(n-3) (n ≥ 4) with a(1) =
 *            1, a(2) = 8, a(3) = 86
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* store computed terms (a(1) → cache[0]) */
static idx_t  cache_len = 0;  /* number of terms currently cached */

/* Compute the nth term of sequence A268052 */
seq_t a(idx_t n)
{
    if (n == 0)                      /* not part of the sequence, but safe */
        return (seq_t)0;

    /* Initialize cache with the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)8;   /* a(2) */
        cache[2] = (seq_t)86;  /* a(3) */
    }

    /* Extend cache if necessary */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */
        cache = tmp;

        /* Fill in missing terms using the recurrence */
        for (idx_t i = cache_len; i < n; ++i) {
            /* i corresponds to term a(i+1) */
            cache[i] = 21 * cache[i-1] + 140 * cache[i-3] - 108 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n-1];   /* return requested term */
}
