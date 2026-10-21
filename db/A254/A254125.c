/*
 * A254125  The number of tilings of a 4 X n rectangle using integer length
 *          rectangles with at least one side of length 1, i.e., tiles are 1 X
 *          1, 1 X 2, ..., 1 X n, 2 X 1, 3 X 1, 4 X 1.
 * https://oeis.org/A254125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 202*a(n-2) + 396*a(n-3) - 248*a(n-4) +
 *            32*a(n-5) with a(0)=1, a(1)=8, a(2)=124, a(3)=2408, a(4)=50128
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms   */
static size_t cache_size  = 0;       /* number of terms actually filled */
static size_t cache_capacity = 0;    /* allocated capacity (in terms) */

/* Allocate or enlarge the cache to at least n+1 elements. */
static void ensure_capacity(idx_t n)
{
    idx_t needed = n + 1;                           /* indices 0..n */
    if ((size_t)needed <= cache_capacity) return;  /* already large enough */

    size_t new_cap = cache_capacity > 0 ? cache_capacity << 1 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed, terminate */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute the n-th term of A254125 using the recurrence. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)8;
        } else if (k == 2) {
            cache[2] = (seq_t)124;
        } else if (k == 3) {
            cache[3] = (seq_t)2408;
        } else if (k == 4) {
            cache[4] = (seq_t)50128;
        } else {
            seq_t v1 = cache[k - 1];
            seq_t v2 = cache[k - 2];
            seq_t v3 = cache[k - 3];
            seq_t v4 = cache[k - 4];
            seq_t v5 = cache[k - 5];

            /* a(k) = 30*v1 - 202*v2 + 396*v3 - 248*v4 + 32*v5 */
            seq_t val = (seq_t)30 * v1;
            val -= (seq_t)202 * v2;
            val += (seq_t)396 * v3;
            val -= (seq_t)248 * v4;
            val += (seq_t)32  * v5;

            cache[k] = val;
        }
    }
    cache_size = (size_t)(n + 1);
    return cache[(size_t)n];
}
