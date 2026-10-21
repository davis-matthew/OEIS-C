/*
 * A254702  Number of length 4+4 0..n arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254702
 * a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
 *        + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)   (n > 8)
 *
 * initial values:
 * a(1)=86, a(2)=1140, a(3)=6808, a(4)=26759, a(5)=81390,
 * a(6)=208114, a(7)=469376, a(8)=962541
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for already computed terms (indices are 1‑based) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated array capacity (elements) */
static size_t cache_len = 0;   /* highest index that is guaranteed valid + 1 */

seq_t a(idx_t n)
{
    /* convert idx_t to size_t for array handling (n <= 10000 in tests) */
    const size_t sn = (size_t)n;

    /* ensure the array is large enough to contain index sn */
    if (sn + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= sn) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first eight terms exactly once */
    if (cache_len <= 8) {
        if (cache_len == 0) {
            cache[1] = (seq_t)86;
            cache[2] = (seq_t)1140;
            cache[3] = (seq_t)6808;
            cache[4] = (seq_t)26759;
            cache[5] = (seq_t)81390;
            cache[6] = (seq_t)208114;
            cache[7] = (seq_t)469376;
            cache[8] = (seq_t)962541;
        }
        cache_len = 9;                    /* indices 1..8 are now valid */
    }

    /* compute terms up to n if necessary */
    if (sn + 1 > cache_len) {
        for (size_t i = cache_len; i <= sn; ++i) {
            seq_t t = cache[i-1] * (seq_t)8
                    + cache[i-3] * (seq_t)56
                    + cache[i-5] * (seq_t)56
                    + cache[i-7] * (seq_t)8;

            seq_t s = cache[i-2] * (seq_t)28
                    + cache[i-4] * (seq_t)70
                    + cache[i-6] * (seq_t)28
                    + cache[i-8];

            cache[i] = t - s;           /* recurrence */
        }
        cache_len = sn + 1;
    }

    return cache[sn];
}
