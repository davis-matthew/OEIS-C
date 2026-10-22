/*
 * A378502  a(n) is the number whose base-4 digits are 1 followed by the first
 *          n-1 terms of the periodic sequence with initial period 3,2,0.
 * https://oeis.org/A378502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((17/36) * 4^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (equivalent): a(1) = 1, a(n) = 4*a(n-1) + p[(n-2)%3] where p = {3,2,0}. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache, cache[0] unused */
static idx_t cache_len = 0;          /* largest index currently stored */
static const seq_t period[3] = { (seq_t)3, (seq_t)2, (seq_t)0 };

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset – not checked */

    if (n > cache_len) {
        /* Allocate space for indices up to n (including unused 0). */
        size_t new_len = (size_t)n + 1;    /* new_len is size_t for realloc */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: return 0 (undefined case). */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute and store each new term. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                size_t pidx = (size_t)(i - 2) % 3;
                cache[i] = cache[i - 1] * (seq_t)4 + period[pidx];
            }
        }
        cache_len = n;
    }
    return cache[n];
}
