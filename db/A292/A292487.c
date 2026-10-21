/*
 * A292487  p-INVERT of the odd positive integers, where p(S) = 1 - S - 4 S^2.
 * https://oeis.org/A292487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=44, a(3)=212, a(4)=1020 a(n)=5*a(n-1) -
 *            3*a(n-2) + 11*a(n-3) + 4*a(n-4) for n>=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed terms */
static idx_t cache_len = 0;      /* number of terms stored (0..cache_len-1) */

/* Return the n-th term of sequence A292487 */
seq_t a(idx_t n)
{
    /* initialise cache with the first five terms if necessary */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)44;
        cache[3] = (seq_t)212;
        cache[4] = (seq_t)1020;
    }

    /* If the requested term is already computed, return it */
    if (n < cache_len) return cache[n];

    /* Grow the cache up to n */
    size_t new_len = (size_t)(n + 1);
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                         /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 5 * cache[i-1] - 3 * cache[i-2]
                     + 11 * cache[i-3] + 4 * cache[i-4];
        }
        cache_len = new_len;
    }
    return cache[n];
}
